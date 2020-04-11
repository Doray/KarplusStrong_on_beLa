/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

The platform for ultra-low latency audio and sensor processing

http://bela.io

A project of the Augmented Instruments Laboratory within the
Centre for Digital Music at Queen Mary University of London.
http://www.eecs.qmul.ac.uk/~andrewm

(c) 2016 Augmented Instruments Laboratory: Andrew McPherson,
  Astrid Bin, Liam Donovan, Christian Heinrichs, Robert Jack,
  Giulio Moro, Laurel Pardue, Victor Zappi. All rights reserved.

The Bela software is distributed under the GNU Lesser General Public License
(LGPL 3.0), available here: https://www.gnu.org/licenses/lgpl-3.0.txt

*/
#include <Bela.h>
#include <SampleLoader.h>
#define NUM_CHANNELS 2
#define BEST_N 900;

int telhier = BEST_N;
int telhiertwee = 0;
//int N; // SampleRate / Frequency = 220
int bh = 0;
float gSampleDaat[402];		//SampleRate / Frequency = 220

// frequency = 110×(2^(n/12))
//	sec	3		5		7		8		10		8		7		5		3				12		11
//		130.81	146.83	164.81	174.61	196		174.61	164.81	146.83	130.81
//	
//  N = samplerate / frequency
//		337.13	300.35	267.58	252.56	225		252.56	267.58	300.35	337.13			50.11			100.23
	
int freek[] = {337,300,268,253,225,253,267,300,337};

void ruisje(int N2)
{
	// N = 201;//201;
	srandom(time(0));
	for (int tel = 0; tel <N2; tel++){
		float dobl = ((float)rand()) / ((float)RAND_MAX) * 1.6f - 0.8f;  // noise
		gSampleDaat[tel] = dobl;
	}
};

void pling(BelaContext *context, void *userData, int N)
{
	if (telhier > 0){
	for(unsigned int n = 0; n < context->audioFrames; n++) {
    	    float out = gSampleDaat[bh];
    		audioWrite(context, n, 0, out);
    		audioWrite(context, n, 1, out);
    		int nbh = bh != N-1 ? bh+1 : 0;
    		float avg = 0.999 * 0.5 * (gSampleDaat[bh] + gSampleDaat[nbh]);
    		gSampleDaat[bh] = avg;
    		bh = nbh;
    	}
    	telhier--;
	}
	else
	{
		for(unsigned int n = 0; n < context->audioFrames; n++) {
		audioWrite(context, n, 0, 0);
    	audioWrite(context, n, 1, 0);
		}
		telhiertwee++;
		if (telhiertwee > 8){
			telhiertwee = 0;
			ruisje(freek[telhiertwee]);
			telhier = BEST_N;
			bh = 0;
			}
		else
		{
		ruisje(freek[telhiertwee]);
		telhier = BEST_N;
		bh = 0;
		}
	}
}

bool setup(BelaContext *context, void *userData)
{
	ruisje(freek[telhiertwee]);
	return true;
}

void render(BelaContext *context, void *userData)
{
     pling(context,userData,freek[telhiertwee]);
}

void cleanup(BelaContext *context, void *userData)
{

}

/**
\example sample-loader/render.cpp

Basic Sample Loader
--------------------------------

This example loads a specified range of samples from a file into a buffer using
helper functions provided in SampleLoader.h. This should ideally be used when working
with small wav files. See sampleStreamer and sampleStreamerMulti for more elaborate ways
of loading and playing back larger files.

    for(int ch=0;ch<NUM_CHANNELS;ch++) {
        gSampleData[ch].sampleLen = gEndFrame; //getNumFrames(gFilename); -> zie gEndframe
    	gSampleData[ch].samples = new float[gFrameRange];
       // getSamples(gFilename,gSampleData[ch].samples,ch,gStartFrame,gEndFrame);
    }
*/
