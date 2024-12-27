//------------------------------------------------------------------------
// Copyright(c) 2024 My Plug-in Company.
//------------------------------------------------------------------------

#include "mypluginprocessor.h"
#include "myplugincids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

using namespace Steinberg;

namespace BrownNotes
{
	//------------------------------------------------------------------------
	// BrownianNoteProcessor
	//------------------------------------------------------------------------
	BrownianNoteProcessor::BrownianNoteProcessor() : frequency(440.0), bn(BrownNotes::BrownianNote())
	{
		//--- set the wanted controller for our processor
		setControllerClass(kBrownianNoteControllerUID);
	}

	//------------------------------------------------------------------------
	BrownianNoteProcessor::~BrownianNoteProcessor()
	{
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BrownianNoteProcessor::initialize(FUnknown *context)
	{
		// Here the Plug-in will be instantiated

		//---always initialize the parent-------
		tresult result = AudioEffect::initialize(context);
		// if everything Ok, continue
		if (result != kResultOk)
		{
			return result;
		}

		//--- create Audio IO ------
		// addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
		addAudioOutput(STR16("Mono Out"), Steinberg::Vst::SpeakerArr::kMono);

		/* If you don't need an event bus, you can remove the next line */
		addEventInput(STR16("Event In"), 1);

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BrownianNoteProcessor::terminate()
	{
		// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

		//---do not forget to call parent ------
		return AudioEffect::terminate();
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BrownianNoteProcessor::setActive(TBool state)
	{
		//--- called when the Plug-in is enable/disable (On/Off) -----
		return AudioEffect::setActive(state);
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BrownianNoteProcessor::process(Vst::ProcessData &data)
	{
		//--- First : Read inputs parameter changes-----------

		/*if (data.inputParameterChanges)
		{
			int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
			for (int32 index = 0; index < numParamsChanged; index++)
			{
				if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
				{
					Vst::ParamValue value;
					int32 sampleOffset;
					int32 numPoints = paramQueue->getPointCount ();
					switch (paramQueue->getParameterId ())
					{
					}
				}
			}
		}*/

		Vst::IEventList *eventList = data.inputEvents;
		if (eventList)
		{
			int32 numEvent = eventList->getEventCount();
			for (int32 i = 0; i < numEvent; i++)
			{
				Vst::Event event;
				if (eventList->getEvent(i, event) == kResultOk)
				{
					switch (event.type)
					{
					case Vst::Event::kNoteOnEvent:
						frequency = 440.0 * pow(2.0, (event.noteOn.pitch - 69) / 12.0);
						bn.handle_note_on(frequency);
						break;
					case Vst::Event::kNoteOffEvent:
						bn.handle_note_off();
						break;
					}
				}
			}
		}

		Vst::Sample32 *out = data.outputs[0].channelBuffers32[0];
		bn.process(data.numSamples, out);

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BrownianNoteProcessor::setupProcessing(Vst::ProcessSetup &newSetup)
	{
		//--- called before any processing ----
		bn.initialize(newSetup.sampleRate);
		return AudioEffect::setupProcessing(newSetup);
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BrownianNoteProcessor::canProcessSampleSize(int32 symbolicSampleSize)
	{
		// by default kSample32 is supported
		if (symbolicSampleSize == Vst::kSample32)
			return kResultTrue;

		// disable the following comment if your processing support kSample64
		/* if (symbolicSampleSize == Vst::kSample64)
			return kResultTrue; */

		return kResultFalse;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BrownianNoteProcessor::setState(IBStream *state)
	{
		// called when we load a preset, the model has to be reloaded
		IBStreamer streamer(state, kLittleEndian);

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BrownianNoteProcessor::getState(IBStream *state)
	{
		// here we need to save the model
		IBStreamer streamer(state, kLittleEndian);

		return kResultOk;
	}

	//------------------------------------------------------------------------
}
