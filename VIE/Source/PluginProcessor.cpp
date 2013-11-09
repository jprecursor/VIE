/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic startup code for a Juce application.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "math.h"


//==============================================================================
VIEAudioProcessor::VIEAudioProcessor(): world_x(12), world_y(10), x(0), y(0), state(0), count(0), min_vel(0), max_vel(127), r_division(0.5), r_worldx(1.0), r_worldy(1.0), r_min_vel(0.), r_max_vel(1.), sampCount(0), totalSamples(0), seqOn(false), panicOn(false), m_clockPhase(0), bpm(240), m_currentBeat(0), m_beatsPerBar(4)
{
    for (int i = 0; i<12; i++) {
        for (int j = 0; j<10; j++) {
            for (int k = 0; k<2; k++) {
                world[i][j][k] = 0;
            }
        }
    }
    
    setWorldSizeY(r_worldy);
    setWorldSizeX(r_worldx);
    setMinVel(r_min_vel);
    setMaxVel(r_max_vel);
}

VIEAudioProcessor::~VIEAudioProcessor()
{
}

//==============================================================================

const String VIEAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int VIEAudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float VIEAudioProcessor::getParameter (int index)
{
    switch (index) {
		case DivParam: return r_division;
        case worldSizeParamX:  return r_worldx;
        case worldSizeParamY:  return r_worldy;
        case minVel: return r_min_vel;
        case maxVel: return r_max_vel;
            break;
            
            
		default: return 0.0f;
	}
}

void VIEAudioProcessor::setParameter (int index, float newValue)
{
    switch (index) {
		case DivParam: setDiv(newValue); break;
        case worldSizeParamX:  setWorldSizeX(newValue); break;
        case worldSizeParamY:  setWorldSizeY(newValue); break;
        case minVel:           setMinVel(newValue); break;
        case maxVel:           setMaxVel(newValue); break;
		default: break;
	}
}

const String VIEAudioProcessor::getParameterName (int index)
{
    switch (index) {
		case DivParam:        return "beatDivision";
        case worldSizeParamX: return "worldSizeX";
        case worldSizeParamY: return "worldSizeY";
        case minVel:          return "minVelocity";
        case maxVel:          return "maxVelocity";
			
		default: break;
	}
	
	return String::empty;
}

const String VIEAudioProcessor::getParameterText (int index)
{
    return String(getParameter(index), 2);
}

const String VIEAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String VIEAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool VIEAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool VIEAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool VIEAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool VIEAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int VIEAudioProcessor::getNumPrograms()
{
    return 0;
}

int VIEAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VIEAudioProcessor::setCurrentProgram (int index)
{
}

const String VIEAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void VIEAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void VIEAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_sampleRate = sampleRate;
    setupBar(0, 1);
    setDiv(r_division);
}

void VIEAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void VIEAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)

{
    
    
    
    AudioPlayHead* playHead = getPlayHead();
    
    
    
    if (playHead != nullptr)
        
    {
        
        AudioPlayHead::CurrentPositionInfo position;
        
        playHead->getCurrentPosition(position);
        
        bpm = position.bpm * division;
        
        //std::cout << "Original BPM is " << position.bpm << " " << "Multiplier is " << division <<  " " << "BPM is " << bpm << " ppqPos is " << position.ppqPosition << std::endl;
        
        seqOn = position.isPlaying;
		
		//create iterator to loop through the current midi buffer
		MidiBuffer::Iterator midiIterator(midiMessages);
		
		//create temp buffer where we 'work' and store our transposed midi messages
		MidiBuffer tempMidiBuffer;
		
		//temp variable to store each midi message we iterate through from the buffer
		MidiMessage tempMessage;
		MidiBuffer lifeOut;
		
		//temp variable to store location of each midi msg we iterate through from the buffer
		int midiMessagePos;
		
		//get and store midi msg and its position when there is midi in the next event
		while (midiIterator.getNextEvent(tempMessage, midiMessagePos)) {
			
			//cast y-coordinate using midiNote
			y = tempMessage.getNoteNumber() / world_x;
			
			//cast x-coordinate using midiNote
			x = tempMessage.getNoteNumber() % world_x;
			
			if (tempMessage.getVelocity() > 0) {
				//conway.newEvent(x, y);
				//state = 1;
				world[x][y][1]= 1;
			}
			
			tempMidiBuffer.addEvent(tempMessage, midiMessagePos);
		}
		
		midiMessages = tempMidiBuffer;
        
        
        if (seqOn == true)
        {
            int const numSamples = buffer.getNumSamples();
            
            double const samplesPerBeat = m_sampleRate * 60.0 / bpm;
            
            double beat = 0;
            
            if (m_clockPhase > 0) {
                
                beat = 1 - m_clockPhase;
                
            } else {
                
                beat = 0 - m_clockPhase;
                
            }
            
            for (;;beat += 1)
            {
                int pos = static_cast<int>(beat * samplesPerBeat);
                
                if (pos < numSamples) {
                    if (panicOn) {
                        //midiFlush
                        for (int a = 0; a < 12; a++) {
                            for (int b = 0; b < 10; b++) {
                                int note =  (b*10) + a;
                                MidiMessage midiFlush(0x80, note, 0x00);
                                lifeOut.addEvent(midiFlush, pos);
                            }
                        }
                        panicOn = !panicOn ;
                    }
					
					//lifeOut = conway.update(lifeOut, pos);
     
                    //goes through world array
                    for (int i = 0; i < world_x; i++)
                    {
                        for (int j = 0; j < world_y; j++)
                        {
							
                            if (world[i][j][1] == 1)
                            {
                                world[i][j][0] = 1;
                                
                                int note =  (j*world_x) + i;
                                int velocity;
                                
                                if (min_vel == max_vel) {
                                    velocity = min_vel;
                                }else{
									
                                    velocity = Random().nextInt(max_vel) + min_vel;
                                }
							
                                alpha[i][j] = velocity / 127.0;
                                MidiMessage mOn(0x90, note, velocity);
                                lifeOut.addEvent(mOn, pos);
                            }
                            
                            else if (world[i][j][1] == -1) {
                                
                                world[i][j][0] = 0;
								
                                int note =  (j*world_x) + i;
                                MidiMessage mOff(0x80, note, 0x00);
                                
                                lifeOut.addEvent(mOff, pos);
                                //std::cout<<"added midi:" << note << " " << pos << std::endl;
                                
                            } else {
                                
                                world[i][j][1] = 0;
                            }
                        }
                    }
                    
                    //Birth and death Cycle
                    for (int i = 0; i < world_x; i ++) {
                        for (int j = 0; j < world_y; j++) {
                            
                            count = neighbors(i, j);
					
                            // if dead cell is surrounded by 3, it becomes alive
                            if ( (count == 3) && (world[i][j][0] == 0) ) {
                                
                                world[i][j][1] = 1;
                            }
                            
                            //if alive cell is surrounded by less than 2 or more than 3, it dies
                            
                            if (((count < 2 )||(count > 3)) && (world[i][j][0] == 1)) {
                                
                                world[i][j][1] = -1;
                            }
                        }
                    }
                    
                    m_currentBeat++;
                    m_currentBeat = m_currentBeat % m_beatsPerBar;
                }
                
                else
                {
                    break;
                }
                
                midiMessages = lifeOut;
            }
            
            advanceClock(numSamples);
		
        }else{
            
            setupBar(0, 1);
        }
    }
}


//Coun adjacent cells
int VIEAudioProcessor::neighbors(int x, int y)
{
	return world[(x + 1) % world_x][y][0] +
	world[x][(y + 1) % world_y][0] +
	world[(x + world_x - 1) % world_x][y][0] +
	world[x][(y + world_y - 1) % world_y][0] +
	world[(x + 1) % world_x][(y + 1) % world_y][0] +
	world[(x + world_x - 1) % world_x][(y + 1) % world_y][0] +
	world[(x + world_x - 1) % world_x][(y + world_y - 1) % world_y][0] +
	world[(x + 1) % world_x][(y + world_y - 1) % world_y][0];
}

void VIEAudioProcessor::setupBar(int startingBeat, int beatsPerBar)
{
    m_clockPhase = 0;
    m_currentBeat = startingBeat;
    m_beatsPerBar = beatsPerBar;
}


void VIEAudioProcessor::advanceClock(int numSamples)
{
    double const samplesPerBeat = m_sampleRate * 60 / bpm;
    
    jassert(m_clockPhase >= -.5 && m_clockPhase < .5);
    
    if(m_clockPhase < 0)
    {
        m_clockPhase += 1;
    }
    
    m_clockPhase = m_clockPhase + numSamples / samplesPerBeat;
    
    if (m_clockPhase >= .5) {
        m_clockPhase -= 1;
    }
    
    jassert(m_clockPhase >= -.5 && m_clockPhase < .5);
}

void VIEAudioProcessor::setDiv(float d){
    r_division = d;
    int selector = r_division * 10;
    switch (selector) {
        case 0:
            division = 0.25; // full note
            break;
        case 1:
            division = 0.5; // 1/2 note
            break;
        case 2:
            division = 0.75; // 1/2T note
            break;
        case 3:
            division = 1.0; // 1/4 note
            break;
        case 4:
            division = 1.5; // 1/4T note
            break;
        case 5:
            division = 2.0; // 1/8 note
            break;
        case 6:
            division = 3.0; // 1/8T note
            break;
        case 7:
            division = 4.0; // 1/16 note
            break;
        case 8:
            division = 6.0; // 1/16T note
            break;
        case 9:
            division = 8.0; // 1/32 note
            break;
        case 10:
            division = 12.0; // 1/32T note
            break;
            
        default:
            break;
    }

	VIEAudioProcessorEditor* editor = static_cast<VIEAudioProcessorEditor*> (getActiveEditor());
	if (editor != nullptr) {
		editor->triggerAsyncUpdate();
	}
}

void VIEAudioProcessor::setWorldSizeX (float newworld_x){
    r_worldx = newworld_x;
    world_x = (newworld_x*4) + 8;
    //std::cout<<"new world x is: "<< world_x << std::endl;
    VIEAudioProcessorEditor* editor = static_cast<VIEAudioProcessorEditor*> (getActiveEditor());
	if (editor != nullptr) {
		editor->triggerAsyncUpdate();
	}
}

void VIEAudioProcessor::setWorldSizeY (float newworld_y){
    r_worldy = newworld_y;
    world_y = (newworld_y*2) + 8;
    //std::cout<<"new world y is: "<< world_y << std::endl;
    VIEAudioProcessorEditor* editor = static_cast<VIEAudioProcessorEditor*> (getActiveEditor());
	if (editor != nullptr) {
		editor->triggerAsyncUpdate();
	}
}

void VIEAudioProcessor::setMinVel(float newMin){
    r_min_vel = newMin;
    //std::cout << "New Minimum is" << r_min_vel << std:: endl;
    if (min_vel == 0) {
        min_vel = 20;
    } else  if (min_vel > max_vel){
        min_vel = max_vel;
    } else {
        min_vel = r_min_vel * 127;
    }
    VIEAudioProcessorEditor* editor = static_cast<VIEAudioProcessorEditor*> (getActiveEditor());
	if (editor != nullptr) {
		editor->triggerAsyncUpdate();
	}
}

void VIEAudioProcessor::setMaxVel(float newMax){
    r_max_vel = newMax;
    //std::cout << "New Maximum is" << r_max_vel << std:: endl;
    if (max_vel <= min_vel) {
        max_vel = min_vel + 10;
    } else {
        max_vel = r_max_vel * 127;
    }
    VIEAudioProcessorEditor* editor = static_cast<VIEAudioProcessorEditor*> (getActiveEditor());
	if (editor != nullptr) {
		editor->triggerAsyncUpdate();
	}
}

void VIEAudioProcessor::resetWorld(){
    //initialize all to 0
    panicOn = true;
    for (int i = 0; i < world_x; i++)
    {
        for (int j = 0; j < world_y; j++)
        {
            for (int k = 0; k < 2; k++) {
                world[i][j][k] = 0;
            }
        }
    }
}

//==============================================================================
bool VIEAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* VIEAudioProcessor::createEditor()
{
    return new VIEAudioProcessorEditor (this);
}

//==============================================================================
void VIEAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VIEAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VIEAudioProcessor();
}
