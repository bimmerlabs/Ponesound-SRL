#include <srl.hpp>
#include <ponesound.hpp>

using namespace SRL::Types;
using namespace SRL::Input;
using namespace SRL::Ponesound;

int main()
{
	SRL::Core::Initialize(SRL::Types::HighColor::Colors::Black);
    
    // Ponesound
    Sound::Driver::Initialize(Sound::ADXMode::ADX2304);
    
    Digital port0(0);
    
    SRL::Debug::Print(1,3,  "Loading...");
    int16_t gameOverPcm8 = Pcm::LoadPcm("GMOVR8.PCM", Sound::PCMBitDepth::PCM8, 15360);
    int16_t bumpPcm16    = Pcm::LoadPcm("BUMP16.PCM", Sound::PCMBitDepth::PCM16, 15360);
    int16_t adx4snd      = Pcm::LoadAdx("NBGM.ADX");
    int32_t currentTrack = 2;
    bool playCDDA = false;
    
    int16_t volume = 15;
    
    SRL::Debug::Print(1,4, "Volume %d  ", volume);
    
    SRL::Debug::Print(1,7,  "Controls:");
    SRL::Debug::Print(5,8,  "Start: start/stop CDDA playback");
    SRL::Debug::Print(5,9,  "R/L trigger: previous/next track");
    SRL::Debug::Print(5,10,  "Up/Down: - master volume control");
    SRL::Debug::Print(5,11, "A: - PCM playback (protected)");
    SRL::Debug::Print(5,12, "C: - PCM playback (semi)");
    SRL::Debug::Print(5,13, "X: - ADX playback");
    SRL::Debug::Print(5,14, "Y: - ADX stop");
    

	while(1)
	{
        DateTime time = DateTime::Now();
        SRL::Debug::Print(1,1, "%d:%d:%d %d.%d.%d    ", time.Hour(), time.Minute(), time.Second(), time.Day(), time.Month(), time.Year());
        SRL::Debug::Print(1,3, "numberOfPCMs %d", Sound::GetNumberOfPCMs());

        if (port0.WasPressed(Digital::Button::A))
        {
            SRL::Debug::Print(1,5, "Play sample %d", gameOverPcm8);
            Pcm::Play(gameOverPcm8, Sound::Protected, 7);
        }
        // if (port0.WasPressed(Digital::Button::B))
        // {
            // gameOverPcm8 = Pcm::LoadPcm("GMOVR8.PCM", Sound::PCMBitDepth::PCM8, 15360);
        // }
        if (port0.WasPressed(Digital::Button::C))
        {
            SRL::Debug::Print(1,5, "Play sample %d ", bumpPcm16);
            Pcm::Play(bumpPcm16, Sound::PlayMode::Volatile, 7);
        }
        if (port0.WasPressed(Digital::Button::X))
        {
            SRL::Debug::Print(1,5, "Play adx %d    ", adx4snd);
            Pcm::Play(adx4snd, Sound::PlayMode::Semi, 7);
        }
        if (port0.WasPressed(Digital::Button::Y))
        {
            SRL::Debug::Print(1,5, "Stop adx %d    ", adx4snd);
            Pcm::Stop(adx4snd);
        }
        
        if (port0.WasPressed(Digital::Button::START))
        {
            if (!playCDDA)
            {
                SRL::Debug::Print(1,5, "Play Track %d   ", currentTrack);
                CD::PlaySingle(currentTrack, true);
            }
            else
            {
                SRL::Debug::Print(1,5, "Stop CDDA       ");
                CD::Stop();
            }
            playCDDA = !playCDDA;
        }
        if (port0.WasPressed(Digital::Button::R) && playCDDA)
        {
            currentTrack++;
            if (currentTrack > 5)
            {
                currentTrack = 2;
            }
            SRL::Debug::Print(1,5, "Play Track %d   ", currentTrack);
            CD::PlaySingle(currentTrack, true);
        }
        if (port0.WasPressed(Digital::Button::L) && playCDDA)
        {
            currentTrack--;
            if (currentTrack < 2)
            {
                currentTrack = 5;
            }
            SRL::Debug::Print(1,5, "Play Track %d   ", currentTrack);
            CD::PlaySingle(currentTrack, true);
        }
        
        if (port0.WasPressed(Digital::Button::Up))
        {
            volume++;
            if (volume > 15)
            {
                volume = 15;
            }
            SRL::Debug::Print(1,4, "Volume %d  ", volume);
            Sound::Driver::SetMasterVolume(volume);
        }
        if (port0.WasPressed(Digital::Button::Down))
        {
            volume--;
            if (volume < 0)
            {
                volume = 0;
            }
            SRL::Debug::Print(1,4, "Volume %d  ", volume);
            Sound::Driver::SetMasterVolume(volume);
        }
        
        SRL::Core::Synchronize();
	}

	return 0;
}
