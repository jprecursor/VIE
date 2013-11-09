/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_24AF6786__
#define __PLUGINPROCESSOR_H_24AF6786__

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class VIEAudioProcessor  :	public AudioProcessor
{
public:
    //==============================================================================
    VIEAudioProcessor();
    ~VIEAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    
    /** Returns true if a silent input always produces a silent output. */
    bool silenceInProducesSilenceOut()const {return false;};
    
    /** Returns the length of the filter's tail, in seconds. */
    double getTailLengthSeconds() const {return 0;};

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
	
    enum Parameters {
		DivParam = 0,
        worldSizeParamX,
        worldSizeParamY,
        minVel,
        maxVel,
        
		totalNumParams
	};
    	    
    int getState(int x, int y, int z){return world[x][y][z];};
    float getAlpha(int x, int y){ return alpha[x][y];};
    
    int getWorldX(){return world_x;};
    int getWorldY(){return world_y;};
    
    float getDiv(){return r_division;};
    float getMinVel(){return min_vel/127.;};
    float getMaxVel(){return max_vel/127.;};
    
    void resetWorld();
	
private:
    
	
    int world_x, world_y, x, y, state, count, min_vel, max_vel;
    float division, r_division, r_worldx, r_worldy, r_min_vel, r_max_vel;
	
	//Conway conway;
    
	int sampRate, sampCount, totalSamples;
	int neighbors(int x, int y);
    
    int world[12][10][2];
    float alpha[12][10];
        
    void setDiv (float d);
    void setWorldSizeX (float newworld_x);
    void setWorldSizeY (float newworld_y);
    void setMinVel (float newMin);
    void setMaxVel (float newMax);
    
    bool seqOn;
    bool panicOn;
    
    //Clock bpm-synced
	void setupBar(int startingBeat, int beatsPerBar);
    void advanceClock(int numSamples);
    float m_sampleRate, m_clockPhase;
    float bpm;
    int m_currentBeat, m_beatsPerBar;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VIEAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_24AF6786__
