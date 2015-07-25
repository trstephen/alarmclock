#include "audioMP3.h"
#include "core_cm4.h"
#include "stm32f4xx_conf.h"
#include "mp3dec.h"
#include "Audio.h"
#include <string.h>
#include "main.h"

// Macros
#define f_tell(fp)		((fp)->fptr)

// Variables
volatile uint32_t		time_var1, time_var2;
USB_OTG_CORE_HANDLE		USB_OTG_Core;
USBH_HOST				USB_Host;
RCC_ClocksTypeDef		RCC_Clocks;
volatile int			enum_done = 0;

// MP3 Variables
#define FILE_READ_BUFFER_SIZE   2000
#define AUDIO_LOCAL_BUFFER_SIZE 2304
#define NUMBER_BUFFERS			12
#define AUDIO_INT_BUFFER		2304

MP3FrameInfo			mp3FrameInfo;
HMP3Decoder				hMP3Decoder;
FIL						file;
int16_t					audio_read_buffer[AUDIO_LOCAL_BUFFER_SIZE * NUMBER_BUFFERS];
uint					audio_buffer_length[NUMBER_BUFFERS];
volatile uint			buffer_read;
volatile uint			buffer_write;
bool                    running_player;

// Private function prototypes
static void AudioCallback(void *context,int buffer);
static uint32_t Mp3ReadId3V2Tag(FIL* pInFile, char* pszArtist,
		uint32_t unArtistSize, char* pszTitle, uint32_t unTitleSize);
static bool fill_mp3_buffer(FIL *fp, int buffer_number, bool reset);
static void play_mp3(char* filename);
static FRESULT play_directory (const char* path, unsigned char seek);



/*
 * Main function. Called when startup code is done with
 * copying memory and setting up clocks.
 */
int audioToMp3(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// SysTick interrupt each 1ms
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	// GPIOD Peripheral clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Initialize USB Host Library
	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_Callbacks);

	for(;;) {
		if(0==exitMp3)
		{
			USBH_Process(&USB_OTG_Core, &USB_Host);

			if (enum_done >= 2) {
				enum_done = 0;
				play_directory("", 0);
			}
		}
		//exit and disable peripherals
		else
		{
			//RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
			DAC_Cmd(DAC_Channel_1, DISABLE);
			DAC_DMACmd(DAC_Channel_1, DISABLE); //DMAEN1
			DMA_Cmd(DMA1_Stream5, DISABLE);
			TIM_Cmd(TIM6, DISABLE);


			exitMp3 = 0;
			mp3PlayingFlag = 0;
			break;
		}
	}
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

static FRESULT play_directory (const char* path, unsigned char seek) {
	FRESULT res;
	FILINFO fno;
	DIR dir;
	char *fn; /* This function is assuming non-Unicode cfg. */
	char buffer[200];
#if _USE_LFN
	static char lfn[_MAX_LFN + 1];
	fno.lfname = lfn;
	fno.lfsize = sizeof(lfn);
#endif


	res = f_opendir(&dir, path); /* Open the directory */
	if (res == FR_OK) {
		for (;;) {

			if(0 == exitMp3)
			{
				res = f_readdir(&dir, &fno); /* Read a directory item */
				if (res != FR_OK || fno.fname[0] == 0) break; /* Break on error or end of dir */
				if (fno.fname[0] == '.') continue; /* Ignore dot entry */
	#if _USE_LFN
				fn = *fno.lfname ? fno.lfname : fno.fname;
	#else
				fn = fno.fname;
	#endif
				if (fno.fattrib & AM_DIR) { /* It is a directory */

				} else { /* It is a file. */
					sprintf(buffer, "%s/%s", path, fn);

					// Check if it is an mp3 file
					if (strcmp("MP3", get_filename_ext(buffer)) == 0) {


						// Skip "seek" number of mp3 files...
						if (seek) {
							seek--;
							continue;
						}

						play_mp3(buffer);
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	return res;
}

/*
static int fill_wav_buffer(FIL *fp) {
	unsigned int br,btr;
	static int16_t    file_read_buffer[FILE_READ_BUFFER_SIZE];
	static int16_t    *file_read_ptr = file_read_buffer;
    static uint       number_samples = 0;

	FRESULT        res;
	int            err;
	int            outOfData = 0;
	int            copy_samples;

	// Turn on blue LED to indicate decoding start.
	GPIO_SetBits(GPIOD, GPIO_Pin_15);

	// Disable audio callback interrupt.
	NVIC_DisableIRQ(DMA1_Stream7_IRQn);

	// Copy audio samples to front of the buffer
	copy_samples = write_ptr - read_ptr;
	memmove(audio_read_buffer, read_ptr, sizeof(int16_t) * copy_samples);
	write_ptr = audio_read_buffer + copy_samples;
	read_ptr = audio_read_buffer;

	// Copy extra samples into buffer
	memcpy(write_ptr,file_read_buffer, sizeof(int16_t) * number_samples );
	write_ptr += number_samples;

	// Enable audio callback interrupt.
	NVIC_EnableIRQ(DMA1_Stream7_IRQn);

	do {
		// Fill the file read buffer
		btr = sizeof(int16_t) * FILE_READ_BUFFER_SIZE;
		res = f_read(fp, file_read_buffer , btr, &br);
		number_samples = br/sizeof(int16_t);

		if ( write_ptr - audio_read_buffer + number_samples > AUDIO_READ_BUFFER_SIZE ) {
			copy_samples = AUDIO_READ_BUFFER_SIZE - ( write_ptr - audio_read_buffer );
		} else {
			copy_samples = number_samples;
		}

		memcpy(write_ptr,file_read_buffer,sizeof(int16_t) * copy_samples );
		write_ptr += copy_samples;
		number_samples -= copy_samples;


		if ( number_samples > 0  ) {
			memmove(file_read_buffer,file_read_buffer + copy_samples, sizeof(int16_t) * number_samples);
		}

		outOfData = f_eof(fp);

	} while( res == FR_OK && !outOfData && number_samples == 0  );

	// Turn off blue LED to indicate decoding finish.
	GPIO_ResetBits(GPIOD, GPIO_Pin_15);

	return(outOfData);
}
*/

static bool fill_mp3_buffer(FIL *fp, int buffer_number, bool reset) {

	unsigned int   br;
	unsigned int   btr;

	static char    file_read_buffer[FILE_READ_BUFFER_SIZE];
	static char *  file_read_ptr = file_read_buffer;
	static uint    file_bytes = 0;

	// Copy of above pointers/indicators for roll back if MP3 frame decode failure.
	char *         copy_file_read_ptr;
	uint           copy_file_bytes;

	bool   		   need_data= true;

	FRESULT res;
	int offset,err;
	bool out_of_data = false;
    int dc_offset;
	int copy_samples;

	int i;

	if ( reset ) {
		file_read_ptr = file_read_buffer;
		file_bytes = 0;
	}

	// Turn on blue LED to indicate decoding start.
	GPIO_SetBits(GPIOD, GPIO_Pin_15);

	audio_buffer_length[ buffer_number ] = 0;

	do {
		if ( need_data ) {
			// Reset the file read buffer
			memmove(file_read_buffer,file_read_ptr,file_bytes);
			file_read_ptr = file_read_buffer;

			// Fill the file read buffer
			btr =      FILE_READ_BUFFER_SIZE - file_bytes;
			res =      f_read(fp, file_read_buffer + file_bytes , btr, &br);
			file_bytes += br;

			if ( res != FR_OK ) {
				// File access failure, dump out.
				out_of_data  = true;
				break;
			}

			// Flag when file is completely read complete.
			out_of_data = f_eof(fp);

			// Clear flag for needing more data.
			need_data = false;
		}

		// Find the next frame in the MP3 file
		offset = MP3FindSyncWord((unsigned char*)file_read_ptr, file_bytes);

		if ( offset < 0 ) {
			// This should never happen unless we have a badly formatted frame.
			// Just exit out and hope the next call gets a good frame.
			need_data = true;
			continue;
		}

		copy_file_bytes    = file_bytes - offset;
		copy_file_read_ptr = file_read_ptr + offset;

		// Decode this frame.
		err = MP3Decode(hMP3Decoder, (unsigned char**)&copy_file_read_ptr, (uint*)&copy_file_bytes, audio_read_buffer + buffer_number * AUDIO_LOCAL_BUFFER_SIZE, 0);

		if (err) {
			/* error occurred */
			switch (err) {
			case ERR_MP3_INDATA_UNDERFLOW:
				// Next loop will refill buffer.
				need_data = true;
				continue;
			case ERR_MP3_MAINDATA_UNDERFLOW:
				// Next loop will refill buffer.
				need_data = true;
				continue;
			case ERR_MP3_FREE_BITRATE_SYNC:
			default:
				out_of_data = true;
				break;
			}
		} else {

			file_bytes    = copy_file_bytes;
			file_read_ptr = copy_file_read_ptr;

			// no error
			MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);

			// Duplicate data in case of mono to maintain playback speed
			if (mp3FrameInfo.nChans == 1) {
				dc_offset = mp3FrameInfo.outputSamps/2;
				for(i = mp3FrameInfo.outputSamps-1;i >= 0;i--) 	{
					audio_read_buffer[ buffer_read * AUDIO_LOCAL_BUFFER_SIZE + 2 * i]     = audio_read_buffer[ buffer_read * AUDIO_LOCAL_BUFFER_SIZE + i];
					audio_read_buffer[ buffer_read * AUDIO_LOCAL_BUFFER_SIZE + 2 * i + 1] = audio_read_buffer[ buffer_read * AUDIO_LOCAL_BUFFER_SIZE + i];
				}
				mp3FrameInfo.outputSamps *= 2;
			}

			audio_buffer_length[ buffer_number ] = mp3FrameInfo.outputSamps;

		}

	} while( res == FR_OK && !out_of_data && ( offset < 0 || audio_buffer_length[ buffer_number ] == 0 ) );

	// Turn off blue LED to indicate decoding finish.
	GPIO_ResetBits(GPIOD, GPIO_Pin_15);
	return(out_of_data);
}

static void play_mp3(char* filename) {

	bool out_of_data;
	int cc;


	// Open file
	if (FR_OK == f_open(&file, filename, FA_OPEN_EXISTING | FA_READ)) {

		// Read ID3v2 Tag
		char szArtist[120];
		char szTitle[120];
		Mp3ReadId3V2Tag(&file, szArtist, sizeof(szArtist), szTitle, sizeof(szTitle));


		///////////////////////////////////////////////////////////buffer starts getting filled for first time
		// Start Initial fill of buffer
		hMP3Decoder = MP3InitDecoder();
		out_of_data = fill_mp3_buffer(&file,0,true);
		for (cc = 1 ; cc < NUMBER_BUFFERS ; cc++ ) {
			if ( out_of_data ) {
							break;
			}
			out_of_data = fill_mp3_buffer(&file,cc,false);
		}

		// Initialize buffer counters
		buffer_read = 0;
		buffer_write = 0;

/////////////////////////////////////////////////////////////////////////////////////////
		// Play mp3
		running_player = true;
		InitializeAudio(Audio44100HzSettings);
		// InitializeAudio(Audio32000HzSettings);
		SetAudioVolume(0xAF);
		PlayAudioWithCallback(AudioCallback, 0);
//////////////////////////////////////////////////////////////////////////////////////////
		for(;;) {
			/*
			 *  If we have an unused buffer, call fill_mp3_buffer to fill it.
			 */
			if ( buffer_read != buffer_write ) {


				// Refill the MP3 buffer
				out_of_data = fill_mp3_buffer(&file,buffer_write,false);

				if ( !out_of_data ) {
					buffer_write = ( buffer_write + 1 ) % NUMBER_BUFFERS;
				}

				// Out of data or error or user button... Stop playback!
				if (out_of_data || (exitMp3 == 1))
				{
					StopAudio();
					running_player = false;

					// Re-initialize and set volume to avoid noise
					InitializeAudio(Audio44100HzSettings);
					SetAudioVolume(0);

					// Close currently open file
					f_close(&file);
					MP3FreeDecoder(hMP3Decoder);
					return;
				}


			}
			else {
				// We don't have any work to do, shut down until interrupt (DMA transfer complete)
				__asm__ volatile ("wfi");
			}
		}
	}
}

/*
 * Called by the audio driver when it is time to provide data to
 * one of the audio buffers (while the other buffer is sent to the
 * CODEC using DMA). One mp3 frame is decoded at a time and
 * provided to the audio driver.
 */



static void AudioCallback(void *context, int buffer) {

	// Audio sample buffers.
	static int16_t audio_buffer0[AUDIO_INT_BUFFER];
	static int16_t audio_buffer1[AUDIO_INT_BUFFER];

	int16_t *samples;
	int16_t *samplesmath;
	int copy_samples;
	int loop=0;
	int16_t *temp;

	if (buffer) {

		samples = audio_buffer0;
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
	} else {

		samples = audio_buffer1;
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	}


	// Determine number of samples to copy
	copy_samples = audio_buffer_length[buffer_read];


	if ( copy_samples > AUDIO_INT_BUFFER ) {
		copy_samples = AUDIO_INT_BUFFER;
	}

	memcpy(samples, audio_read_buffer + buffer_read * AUDIO_LOCAL_BUFFER_SIZE, sizeof(int16_t) * copy_samples );


	//changes data from 16 to 12 bits and level shifts so that it is never below 0.
	samplesmath = samples;
	for(loop=0;loop<copy_samples;loop++)
	{
		*samplesmath = (*samplesmath >> 4 ) + 2048;
		samplesmath++;

	}


	//merges the left and right channels to mono by taking the average
	samplesmath = samples;
	temp = samples;

	for(loop=0;loop<copy_samples/2;loop++)
	{
		*samplesmath = (*temp + *(temp+1))/2;
		samplesmath++;
		temp+=2;
	}

	buffer_read = ( buffer_read + 1 ) % NUMBER_BUFFERS;

	if ( running_player ) {
		ProvideAudioBuffer(samples, copy_samples); // mp3FrameInfo.outputSamps);
	}

}

/*
 * Called by the SysTick interrupt
 */
void TimingDelay_Decrement(void) {
	if (time_var1) {
		time_var1--;
	}
	time_var2++;
}

/*
 * Delay a number of systick cycles (1ms)
 */
void Delay(volatile uint32_t nTime) {
	time_var1 = nTime;
	while(time_var1){};
}

/*
 * Dummy function to avoid compiler error
 */
void _init() {

}

/*
 * Taken from
 * http://www.mikrocontroller.net/topic/252319
 */
static uint32_t Mp3ReadId3V2Text(FIL* pInFile, uint32_t unDataLen, char* pszBuffer, uint32_t unBufferSize)
{
	UINT unRead = 0;
	BYTE byEncoding = 0;
	if((f_read(pInFile, &byEncoding, 1, &unRead) == FR_OK) && (unRead == 1))
	{
		unDataLen--;
		if(unDataLen <= (unBufferSize - 1))
		{
			if((f_read(pInFile, pszBuffer, unDataLen, &unRead) == FR_OK) ||
					(unRead == unDataLen))
			{
				if(byEncoding == 0)
				{
					// ISO-8859-1 multibyte
					// just add a terminating zero
					pszBuffer[unDataLen] = 0;
				}
				else if(byEncoding == 1)
				{
					// UTF16LE unicode
					uint32_t r = 0;
					uint32_t w = 0;
					if((unDataLen > 2) && (pszBuffer[0] == 0xFF) && (pszBuffer[1] == 0xFE))
					{
						// ignore BOM, assume LE
						r = 2;
					}
					for(; r < unDataLen; r += 2, w += 1)
					{
						// should be acceptable for 7 bit ascii
						pszBuffer[w] = pszBuffer[r];
					}
					pszBuffer[w] = 0;
				}
			}
			else
			{
				return 1;
			}
		}
		else
		{
			// we won't read a partial text
			if(f_lseek(pInFile, f_tell(pInFile) + unDataLen) != FR_OK)
			{
				return 1;
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

/*
 * Taken from
 * http://www.mikrocontroller.net/topic/252319
 */
static uint32_t Mp3ReadId3V2Tag(FIL* pInFile, char* pszArtist, uint32_t unArtistSize, char* pszTitle, uint32_t unTitleSize)
{
	pszArtist[0] = 0;
	pszTitle[0] = 0;

	BYTE id3hd[10];
	UINT unRead = 0;
	if((f_read(pInFile, id3hd, 10, &unRead) != FR_OK) || (unRead != 10))
	{
		return 1;
	}
	else
	{
		uint32_t unSkip = 0;
		if((unRead == 10) &&
				(id3hd[0] == 'I') &&
				(id3hd[1] == 'D') &&
				(id3hd[2] == '3'))
		{
			unSkip += 10;
			unSkip = ((id3hd[6] & 0x7f) << 21) | ((id3hd[7] & 0x7f) << 14) | ((id3hd[8] & 0x7f) << 7) | (id3hd[9] & 0x7f);

			// try to get some information from the tag
			// skip the extended header, if present
			uint8_t unVersion = id3hd[3];
			if(id3hd[5] & 0x40)
			{
				BYTE exhd[4];
				f_read(pInFile, exhd, 4, &unRead);
				size_t unExHdrSkip = ((exhd[0] & 0x7f) << 21) | ((exhd[1] & 0x7f) << 14) | ((exhd[2] & 0x7f) << 7) | (exhd[3] & 0x7f);
				unExHdrSkip -= 4;
				if(f_lseek(pInFile, f_tell(pInFile) + unExHdrSkip) != FR_OK)
				{
					return 1;
				}
			}
			uint32_t nFramesToRead = 2;
			while(nFramesToRead > 0)
			{
				char frhd[10];
				if((f_read(pInFile, frhd, 10, &unRead) != FR_OK) || (unRead != 10))
				{
					return 1;
				}
				if((frhd[0] == 0) || (strncmp(frhd, "3DI", 3) == 0))
				{
					break;
				}
				char szFrameId[5] = {0, 0, 0, 0, 0};
				memcpy(szFrameId, frhd, 4);
				uint32_t unFrameSize = 0;
				uint32_t i = 0;
				for(; i < 4; i++)
				{
					if(unVersion == 3)
					{
						// ID3v2.3
						unFrameSize <<= 8;
						unFrameSize += frhd[i + 4];
					}
					if(unVersion == 4)
					{
						// ID3v2.4
						unFrameSize <<= 7;
						unFrameSize += frhd[i + 4] & 0x7F;
					}
				}

				if(strcmp(szFrameId, "TPE1") == 0)
				{
					// artist
					if(Mp3ReadId3V2Text(pInFile, unFrameSize, pszArtist, unArtistSize) != 0)
					{
						break;
					}
					nFramesToRead--;
				}
				else if(strcmp(szFrameId, "TIT2") == 0)
				{
					// title
					if(Mp3ReadId3V2Text(pInFile, unFrameSize, pszTitle, unTitleSize) != 0)
					{
						break;
					}
					nFramesToRead--;
				}
				else
				{
					if(f_lseek(pInFile, f_tell(pInFile) + unFrameSize) != FR_OK)
					{
						return 1;
					}
				}
			}
		}
		if(f_lseek(pInFile, unSkip) != FR_OK)
		{
			return 1;
		}
	}

	return 0;
}

