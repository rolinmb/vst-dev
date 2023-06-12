Collection of projects made whilst learning to build VST3 plugins using the standard Steinberg Media VST3 SDK, as well as the JUCE C++ library more recently.

These folders are just the raw source code files from the project/solution generators; you will have to follow the documation and copy the code into a generated project file if you were to try and build any of this code.

Use at your own risk; not all of these work as advertised, some memory leaks may occur (which result in loud noise emitted in your DAW/wherever your VST plugin is being hosted). I recommend following some of the same tutorials that I went through to get started.

The YouTube channel (aikelab)[https://www.youtube.com/@aikelab/featured] has two videos I studied to attempt to create all of the VST plugins (FX and instruments) projects in the /steinberg_sdk folder.

I followed TheAudioProgrammer on [YouTube](https://www.youtube.com/c/TheAudioProgrammer) and [Github](https://github.com/TheAudioProgrammer) for many of their JUCE tutorials. They have a good design pattern for setting up the variables and connections between the user interface and the actual audio processor parts which comprise a VST plugin.

[keithhearne/VSTPlugins](https://github.com/keithhearne/VSTPlugins/tree/master) is another repo with more interesting examples than ones I have created so far, and I will probably try to implement some of these in the future.

I would combine the GUI and Processor setup that TheAudioProgrammer uses while refactoring other projects to get started; or try and copy a more complex project and attempt to get it working.