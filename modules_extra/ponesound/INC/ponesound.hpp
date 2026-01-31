#pragma once
#include <srl.hpp>
#include <smpc.hpp>

using namespace SRL::Math::Types;

namespace SRL::Ponesound
{    
	/**
	 * @brief Class for managing sound operations.
	 */ 
	class Sound final
	{
	private:
        /**
         * @brief Masking function for extracting the least significant N bits of a value.
         * @tparam N Number of bits to extract.
         * @param value Input value.
         * @return Masked value containing the least significant N bits.
         */
        template <int N>
        static auto ExtractLeastSignificantBits(auto value)
        {
            return ((1 << N) - 1) & value;
        }

		/**
		 * @brief Calculates the greatest common divisor (GCD) of two integers.
		 *
		 * This function uses Euclid's algorithm to compute the GCD recursively.
		 *
		 * @param a The first integer.
		 * @param b The second integer.
		 * @return The greatest common divisor of `a` and `b`.
		 */
		static int16_t CalculateGCD(int16_t a, int16_t b)
		{
			return a == 0 ? b : CalculateGCD(b % a, a);
		}

		/**
		 * @brief Calculates the least common multiple (LCM) of two integers.
		 *
		 * The LCM of two numbers `a` and `b` can be computed using the relationship:
		 * CalculateLCM(a, b) = (a / GCD(a, b)) * b
		 *
		 * @param a The first integer.
		 * @param b The second integer.
		 * @return The least common multiple of `a` and `b`.
		 */
		static int16_t CalculateLCM(int16_t a, int16_t b)
		{
			return (a / CalculateGCD(a, b)) * b;
		}

		/**
		 * @brief Struct representing PCM sound parameters.
		 */
		struct PCM
		{
			struct CTRL
			{
				int8_t loopType;
				uint8_t bitDepth;
				uint16_t hiAddrBits;
				uint16_t loAddrBits;
				uint16_t LSA;
				uint16_t playSize;
				uint16_t pitchWord;
				uint8_t pan;
				uint8_t volume;
				uint16_t bytesPerBlank;
				uint16_t decompressionSize;
				uint8_t sh2Permit;
				int8_t icsrTarget;
			};

			static constexpr auto SCSP_FREQUENCY = 44100L;
			static constexpr auto CTRL_MAX = 93;
			static constexpr auto TYPE_ADX = 2;
			static constexpr auto TYPE_8BIT = 1;
			static constexpr auto TYPE_16BIT = 0;
			static constexpr auto SYS_REGION = 0;
			static constexpr auto PAN_LEFT = 1 << 4;
			static constexpr auto PAN_RIGHT = 0;
		};

		static constexpr auto SNDRAM = 631242752;
		static constexpr auto SNDPRG = SNDRAM + 0x408;

		static constexpr auto PCMEND = SNDRAM + 0x7F000;
		static constexpr auto DRV_SYS_END = 47 * 1024;

		/**
		 * @brief Struct representing ADX parameters.
		 */
		struct ADX
		{
			// static constexpr auto STREAM = -3;
			static constexpr auto MASTER_768 = 0;
			static constexpr auto MASTER_1152 = 1;
			static constexpr auto MASTER_1536 = 2;
			static constexpr auto MASTER_2304 = 3;
			static constexpr auto COEF_768_1 = 4401;
			static constexpr auto COEF_768_2 = -1183;
			static constexpr auto COEF_1152_1 = 5386;
			static constexpr auto COEF_1152_2 = -1771;
			static constexpr auto COEF_1536_1 = 5972;
			static constexpr auto COEF_1536_2 = -2187;
			static constexpr auto COEF_2304_1 = 6631;
			static constexpr auto COEF_2304_2 = -2685;
			static constexpr auto PAL_640 = 4;
			static constexpr auto COEF_640_1 = 3915;
			static constexpr auto COEF_640_2 = -936;
			static constexpr auto PAL_960 = 5;
			static constexpr auto COEF_960_1 = 4963;
			static constexpr auto COEF_960_2 = -1504;
			static constexpr auto PAL_1280 = 6;
			static constexpr auto COEF_1280_1 = 5612;
			static constexpr auto COEF_1280_2 = -1923;
			static constexpr auto PAL_1920 = 7;
			static constexpr auto COEF_1920_1 = 6359;
			static constexpr auto COEF_1920_2 = -2469;
		};

		/**
		 * @brief Struct representing system command parameters.
		 */
		struct SystemCommandParameters
		{
			volatile int32_t adxStreamLength;
			volatile uint16_t start;
			volatile int8_t adxBufferPass[2];
			volatile int16_t driverAdxCoeficient1;
			volatile int16_t driverAdxCoeficient2;
			volatile PCM::CTRL* pcmCtrl;
			volatile uint8_t cddaLeftChannelVolPan;
			volatile uint8_t cddaRightChannelVolPan;
		};

		/**
		 * @brief Struct representing ADX header.
		 */
		struct AdxHeader
		{
			uint16_t oneHalf;
			int16_t offset2Data;
			uint8_t format;
			uint8_t blockSize;
			uint8_t bitDepth;
			uint8_t channels;
			uint32_t sampleRate;
			uint32_t sampleCount;
			uint16_t highPassCutOff;
			uint8_t loop;
			uint8_t illegal;
		};

		static inline constexpr int32_t LogaritmicTable[] = {
		 0,
		 1,
		 2, 2,
		 3, 3, 3, 3,
		 4, 4, 4, 4, 4, 4, 4, 4,
		 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
		 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
		};

		static inline auto& m68kCommands = *reinterpret_cast<SystemCommandParameters*> ((SNDPRG + DRV_SYS_END) | 0x20000000);
		static inline auto scspWorkStart = reinterpret_cast<uint32_t*> (0x408 + DRV_SYS_END + 0x20);
		static inline auto& masterVolume = *reinterpret_cast<uint16_t*> (SNDRAM + 0x100400);
		static inline uint32_t* scspWorkAddr;
		static inline uint16_t driverMasterVolume = 0;
		static inline int16_t numberOfPCMs = 0;

		static inline constexpr int16_t adxCoeficientTable[8][2] =
		{
			{ADX::COEF_768_1, ADX::COEF_768_2},
			{ADX::COEF_1152_1, ADX::COEF_1152_2},
			{ADX::COEF_1536_1, ADX::COEF_1536_2},
			{ADX::COEF_2304_1, ADX::COEF_2304_2},
			{ADX::COEF_640_1, ADX::COEF_640_2},
			{ADX::COEF_960_1, ADX::COEF_960_2},
			{ADX::COEF_1280_1, ADX::COEF_1280_2},
			{ADX::COEF_1920_1, ADX::COEF_1920_2}
		};

		static inline AdxHeader adxHeader;

        static void SdrvVblankRq(void)
        {
            m68kCommands.start = 1;
        }

		static void LoadDriver(int32_t masterAdxFrequency)
		{
			*(uint8_t*)(0x25B00400) = 0x02;

            // clear sound ram
 			for (int32_t i = 0; i < 0x80000; i += 4)
			{
				*(uint32_t*)(SNDRAM + i) = 0x00000000;
			}

			SRL::Cd::File file("SDRV.BIN");
		    if (file.Open())
		    {
                SRL::SMPC::DisableSoundCPU();
                file.Read(file.Size.Bytes, (void*)SNDRAM);
                m68kCommands.driverAdxCoeficient1 = adxCoeficientTable[masterAdxFrequency][0];
                m68kCommands.driverAdxCoeficient2 = adxCoeficientTable[masterAdxFrequency][1];
                SRL::SMPC::EnableSoundCPU();

                file.Close();
            }

			m68kCommands.start = 0xFFFF;
			scspWorkAddr = scspWorkStart;
			volatile int32_t i = reinterpret_cast<int32_t>(scspWorkAddr);
            // is this really needed?
			while (i) { i = i - 1; }
			numberOfPCMs = 0;
		}

		static int16_t CalculateBytesPerBlank(int32_t sampleRate, bool is8Bit, bool isPAL)
		{
			int32_t frameCount = isPAL ? 50 : 60;
			int32_t sampleSize = is8Bit ? 8 : 16;
			return ((sampleRate * sampleSize) >> 3) / frameCount;
		}

		static int16_t ConvertBitrateToPitchWord(int16_t sampleRate)
		{
			int32_t octr = ((int32_t)LogaritmicTable[PCM::SCSP_FREQUENCY / ((sampleRate)+1)]);
			int32_t shiftr = PCM::SCSP_FREQUENCY >> octr;
			int32_t fnsr = ((((sampleRate)-(shiftr)) << 10) / (shiftr));
			return ((int32_t)((ExtractLeastSignificantBits<4>(-(octr)) << 11) | ExtractLeastSignificantBits<10>(fnsr)));
		}

	public:
		/**
		 * @brief Enum defining PCM bit depth.
		 */
		enum class PCMBitDepth : int32_t
		{
			/** @brief 8-bit
			 */
			PCM8 = 1,

			/** @brief 16-bit
			 */
			PCM16 = 0
		};

		/**
		 * @brief Enum defining ADX data mode.
		 */
		enum ADXMode : uint32_t
		{
			/** @brief 7.68 Data
			 */
			ADX768 = 0,

			/** @brief 11.52 Data
			 */
			ADX1152 = 1,

			/** @brief 15.36 Data
			 */
			ADX1536 = 2,

			/** @brief 23.04 Data
			 */
			ADX2304 = 3,
		};

		/**
		 * @brief Enum defining sound play mode.
		 */
		enum PlayMode : int32_t
		{
			/** @brief Sound effect will loop, playback direction will change when playback reaches its end
			 */
			AlternatingLoop = 3,

			/** @brief Sound effect will play backwards in a loop
			 */
			ReversLoop = 2,

			/** @brief Sound effect will play normaly in loop
			 */
			ForwardLoop = 1,

			/** @brief Sound effect will play normaly
			 */
			Volatile = 0,

			/** @brief Sound effect will play normaly, but when stopped, it wil not stop playing until its end
			 */
			Protected = -1,

			/** @brief Sound effect will play normaly, but when stopped, it wil not stop playing until its end
			 */
			Semi = -2,
		};

		/** @brief Returns current number of PCMs
		 */
        static int16_t GetNumberOfPCMs()
        {
            return numberOfPCMs;
        }
        
		/** @brief Hardware settings and Driver initialization
		 */
		struct Driver
		{
		    /**
			 * @brief Initializes the sound driver.
			 * @param mode ADX data mode.
			 */
			static void Initialize(const ADXMode mode)
			{
				// Load driver
				LoadDriver(mode);
				SRL::Core::OnVblank += SdrvVblankRq;

                SetMasterVolume(15);

				// set CD audio volume
				CD::SetVolume(15);
			}

			/** @brief Set master volume
			 * @param volume New volume (0-15)
			 */
			static void SetMasterVolume(uint16_t volume)
			{
                volume = volume >= 0xF ? 0xF : volume;
                masterVolume = 0x200 | (volume & 0xF);
                driverMasterVolume = volume;
			}
		};

		/** @brief Playback of sound effects & music
		 */
		struct Pcm
		{		    
			/** @brief Load PCM sound effect
			 * @param file File name
			 * @param bitDepth Bit depth of the sound effect
			 * @param sampleRate Sample rate of the sound effect
			 * @return Sound effect identifier (< 0 on fail)
			 */
			static int16_t LoadPcm(const char* fileName, const PCMBitDepth bitDepth, const int32_t sampleRate)
			{
                if ((int32_t)scspWorkAddr > 0x7F800) return -1;
                if (numberOfPCMs >= PCM::CTRL_MAX) return -2;

                SRL::Cd::File file(fileName);

                if (file.Open())
                {
                    int32_t fileSize = file.Size.Bytes;

                    if (fileSize > (128 * 1024) && bitDepth == PCMBitDepth::PCM16)
                    {
                        file.Close();
                        return -3;
                    }
                    else if (fileSize > (64 * 1024) && bitDepth == PCMBitDepth::PCM8)
                    {
                        file.Close();
                        return -3;
                    }

                    fileSize += ((uint32_t)fileSize & 1) ? 1 : 0;
                    fileSize += ((uint32_t)fileSize & 3) ? 2 : 0;

                    file.Read(fileSize, (void*)((uint32_t)scspWorkAddr + SNDRAM));
                    file.Close();
                    
                    m68kCommands.pcmCtrl[numberOfPCMs].hiAddrBits = (uint16_t)((uint32_t)scspWorkAddr >> 16);
                    m68kCommands.pcmCtrl[numberOfPCMs].loAddrBits = (uint16_t)((uint32_t)scspWorkAddr & 0xFFFF);
                    m68kCommands.pcmCtrl[numberOfPCMs].pitchWord    = ConvertBitrateToPitchWord(sampleRate);

                    if (bitDepth == PCMBitDepth::PCM16)
                    {
                        m68kCommands.pcmCtrl[numberOfPCMs].bytesPerBlank = CalculateBytesPerBlank(sampleRate, false, PCM::SYS_REGION);
                        m68kCommands.pcmCtrl[numberOfPCMs].playSize = (fileSize >> 1);
                        m68kCommands.pcmCtrl[numberOfPCMs].bitDepth = PCM::TYPE_16BIT;
                    }
                    else if (bitDepth == PCMBitDepth::PCM8) {
                        m68kCommands.pcmCtrl[numberOfPCMs].bytesPerBlank = CalculateBytesPerBlank(sampleRate, true, PCM::SYS_REGION);
                        m68kCommands.pcmCtrl[numberOfPCMs].playSize = (fileSize);
                        m68kCommands.pcmCtrl[numberOfPCMs].bitDepth = PCM::TYPE_8BIT;
                    }

                    m68kCommands.pcmCtrl[numberOfPCMs].loopType = 0;
                    m68kCommands.pcmCtrl[numberOfPCMs].volume = 7;

                    numberOfPCMs++;
                    scspWorkAddr = (uint32_t*)((uint32_t)scspWorkAddr + fileSize);
                    
                    return (numberOfPCMs - 1);
                }
                else {
                    return -4;
                }
			}

			/** @brief Load 8 bit PCM sound effect
			 * @param file File name
			 * @param sampleRate Sample rate of the sound effect
			 * @return Sound effect identifier (< 0 on fail)
			 */
			static int16_t Load8(const char* fileName, const int32_t sampleRate = 15360)
			{
			    return LoadPcm(fileName, PCMBitDepth::PCM8, sampleRate);
			}
			
			/** @brief Load 16 bit PCM sound effect
			 * @param file File name
			 * @param sampleRate Sample rate of the sound effect
			 * @return Sound effect identifier (< 0 on fail)
			 */
			static int16_t Load16(const char* fileName, const int32_t sampleRate = 15360)
			{
			    return LoadPcm(fileName, PCMBitDepth::PCM16, sampleRate);
			}
			
			/** @brief Load ADX sound effect
			 * @param fileName File name
			 * @return Sound effect identifier (< 0 on fail)
			 */
			static int16_t LoadAdx(const char* fileName)
			{
                if ((int32_t)scspWorkAddr > 0x7F800) return -1;
                if (numberOfPCMs >= PCM::CTRL_MAX) return -2;

                SRL::Cd::File file(fileName);

                if (file.Open())
                {
                    AdxHeader adxHeader{};

                    if (file.Read(sizeof(AdxHeader), (void*)&adxHeader) &&
                    (adxHeader.oneHalf == 32768 && adxHeader.blockSize == 18 && adxHeader.bitDepth == 4))
                    {
                        uint32_t workAddress = (uint32_t)(scspWorkAddr) + 16;  // we are not copying the header so this offset is different
                        m68kCommands.pcmCtrl[numberOfPCMs].hiAddrBits = (uint16_t)((uint32_t)workAddress >> 16);
                        m68kCommands.pcmCtrl[numberOfPCMs].loAddrBits = (uint16_t)((uint32_t)workAddress & 0xFFFF);
                        m68kCommands.pcmCtrl[numberOfPCMs].pitchWord = ConvertBitrateToPitchWord(adxHeader.sampleRate);
                        m68kCommands.pcmCtrl[numberOfPCMs].playSize = (adxHeader.sampleCount / 32);
                        int16_t bytesPerBlank = CalculateBytesPerBlank((int32_t)adxHeader.sampleRate, false, PCM::SYS_REGION);

                        if (bytesPerBlank != 768 && bytesPerBlank != 512 && bytesPerBlank != 384 && bytesPerBlank != 256 && bytesPerBlank != 192 && bytesPerBlank != 128)
                        {
                            file.Close();
                            return -3;
                        }
                        else {
                            m68kCommands.pcmCtrl[numberOfPCMs].bytesPerBlank = bytesPerBlank;
                            uint16_t bigDictionarySize = (bytesPerBlank >= 256) ? CalculateLCM(bytesPerBlank, bytesPerBlank + 64) << 1 : 5376;
                            m68kCommands.pcmCtrl[numberOfPCMs].decompressionSize = (bigDictionarySize > (adxHeader.sampleCount << 1)) ? adxHeader.sampleCount << 1 : bigDictionarySize;
                            m68kCommands.pcmCtrl[numberOfPCMs].bitDepth = PCM::TYPE_ADX;
                            m68kCommands.pcmCtrl[numberOfPCMs].loopType = PlayMode::Semi;
                            m68kCommands.pcmCtrl[numberOfPCMs].volume = 7;

                            uint32_t bytesToLoad = (adxHeader.sampleCount / 32) * 18;
                            bytesToLoad += ((uint32_t)bytesToLoad & 1) ? 1 : 0;
                            bytesToLoad += ((uint32_t)bytesToLoad & 3) ? 2 : 0;

                            file.Read(bytesToLoad, (void*)((uint32_t)scspWorkAddr + SNDRAM));
                            file.Close();

                            numberOfPCMs++;
                            scspWorkAddr = (uint32_t*)((uint32_t)scspWorkAddr + bytesToLoad);

                            return (numberOfPCMs - 1);
                        }
                    }
                    else {
                        file.Close();
                        return -4;
                    }
                }
                else
                {
                    return -5;
                }
			}

			/** @brief Set volume of currently playing sound
			 * @param sound Sound to modify
			 * @param volume New volume (0-7)
			 * @param pan Stereo pan to set (right being 0, left being 16)
			 */
			static void SetVolume(const int16_t sound, const uint8_t volume, const uint8_t pan = 7)
			{
				if (sound < 0) return;
				m68kCommands.pcmCtrl[sound].volume = volume;
				m68kCommands.pcmCtrl[sound].pan = pan;
			}

			/** @brief Stop playing sound
			 * @param sound Sound to stop
			 */
			static void Stop(const int16_t sound)
			{
				if (sound < 0) return;
				if (m68kCommands.pcmCtrl[sound].loopType <= 0)
				{
					m68kCommands.pcmCtrl[sound].volume = 0;
				}
				else
				{
					m68kCommands.pcmCtrl[sound].sh2Permit = 0;
				}
			}

			/** @brief Will remove all sounds after the specified sound
			 * @param lastToKeep Index of the last sound to be kept loaded
			 */
			static void Unload(const int16_t lastTokeep)
			{
				if (lastTokeep < 0)
				{
					scspWorkAddr = scspWorkStart;
					numberOfPCMs = 0;
					return;
				}

				numberOfPCMs = lastTokeep + 1;
				scspWorkAddr =
					(uint32_t*)((uint32_t)(m68kCommands.pcmCtrl[lastTokeep].hiAddrBits << 16) |
						(int32_t)(m68kCommands.pcmCtrl[lastTokeep].loAddrBits));

				if (m68kCommands.pcmCtrl[lastTokeep].bitDepth == 2)
				{
					scspWorkAddr = (uint32_t*)((uint32_t)scspWorkAddr + (m68kCommands.pcmCtrl[lastTokeep].playSize * 18));
				}
				else if (m68kCommands.pcmCtrl[lastTokeep].bitDepth == 1)
				{
					scspWorkAddr = (uint32_t*)((uint32_t)scspWorkAddr + m68kCommands.pcmCtrl[lastTokeep].playSize);
				}
				else if (m68kCommands.pcmCtrl[lastTokeep].bitDepth == 0)
				{
					scspWorkAddr = (uint32_t*)((uint32_t)scspWorkAddr + (m68kCommands.pcmCtrl[lastTokeep].playSize << 1));
				}
			}

			/** @brief Play sound
			 * @param sound Sound to play
			 * @param mode Loop/Playback mode mode
			 * @param volume Starting volume
			 */
            static void Play(int16_t sound,
            PlayMode mode = PlayMode::Protected,
            uint8_t volume = 15)
            {
				if (sound < 0) return;
				m68kCommands.pcmCtrl[sound].sh2Permit = 1;
				m68kCommands.pcmCtrl[sound].volume = volume;
				m68kCommands.pcmCtrl[sound].loopType = mode;
			}
		};
		
		// /** @brief CD Streamed playback of sound effects & music (future)
		 // */
		// struct PcmStream
		// {
		// };

		/** @brief Playback of CD audio
		 */
		struct CD
		{
			/** @brief Set CD playback volume
			 *  @param volume Driver volume (7 is max)
			 */
			static void SetVolume(const uint8_t volume)
			{
				uint8_t newvol = m68kCommands.cddaLeftChannelVolPan & 0x1F;
				newvol |= ((volume & 0x7) << 5);
				m68kCommands.cddaLeftChannelVolPan = newvol;

				newvol = m68kCommands.cddaRightChannelVolPan & 0x1F;
				newvol |= ((volume & 0x7) << 5);
				m68kCommands.cddaRightChannelVolPan = newvol;
			}

			/** @brief Set CD playback stereo pan
			 *  @param left Left channel volume (7 is max)
			 *  @param right Right channel volume (7 is max)
			 */
			static void SetPan(const uint8_t left, const uint8_t right)
			{
				uint8_t newvol = m68kCommands.cddaLeftChannelVolPan & 0x1F;
				newvol |= ((left & 0x7) << 5);
				m68kCommands.cddaLeftChannelVolPan = newvol;

				newvol = m68kCommands.cddaRightChannelVolPan & 0x1F;
				newvol |= ((right & 0x7) << 5);
				m68kCommands.cddaRightChannelVolPan = newvol;
			}

			/** @brief Play range of tracks
			 *  @param fromTrack Starting track
			 *  @param toTrack Ending track
			 *  @param loop Whether to play the range of track again after it ends
			 */
			static void Play(const int32_t fromTrack, const int32_t toTrack, const bool loop = false)
			{
				CdcPly ply;

                // Start track
                CDC_PLY_STYPE(&ply) = CDC_PTYPE_TNO;
                CDC_PLY_STNO(&ply) = fromTrack;
                CDC_PLY_SIDX(&ply) = 1;

                // End track
                CDC_PLY_ETYPE(&ply) = CDC_PTYPE_TNO;
                CDC_PLY_ETNO(&ply) = toTrack;
                CDC_PLY_EIDX(&ply) = 1;

                // Set loop mode
                CDC_PLY_PMODE(&ply) = CDC_PM_DFL | (loop ? 0xf : 0); // 0xf = infinite repetitions

                CDC_CdPlay(&ply);
			}

			/** @brief Play a single track
			 *  @param loop Whether to play the range of track again after it ends
			 */
			static void PlaySingle(int32_t track, bool loop = false)
			{
				Play(track, track, loop);
			}

			/** @brief Stop CD music playback
			 */
			static void Stop()
			{
				CdcPos poswk;
				poswk.ptype = CDC_PTYPE_DFL;
				CDC_CdSeek(&poswk);
			}
		};
	};
    
    /** 
     * @brief PCM BitDepth alias
     */
    using BitDepth = Sound::PCMBitDepth;
    
    /** 
     * @brief PCM API alias
     */
    using Pcm = Sound::Pcm;

    /**
     * @brief CD API alias
     */
    using CD = Sound::CD;
}
