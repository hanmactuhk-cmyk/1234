
#include <JuceHeader.h>
#include "MainComponent.h"

class HNStudioApp : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "HNStudio Musik AI"; }
    const juce::String getApplicationVersion() override { return "0.4.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String&) override
    {
        mainWindow = std::make_unique<MainWindow>();
    }

    void shutdown() override { mainWindow.reset(); }

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow() : DocumentWindow("HNStudio Musik AI",
            juce::Colours::black, DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setResizable(true, true);
            setContentOwned(new MainComponent(), true);
            centreWithSize(1050, 720);
            setVisible(true);
        }
        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };
private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(HNStudioApp)
