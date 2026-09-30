
#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(1050,720);
    title.setText("✦ HNSTUDIO MUSIK AI ✦", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::centred);
    title.setFont(juce::FontOptions(24.0f).withStyle("bold"));
    addAndMakeVisible(title);

    addAndMakeVisible(live);
    live.onClick=[this]{ setLive(); };

    addAndMakeVisible(stereo);
    stereo.onClick=[this]{ toggleStereoMix(); };

    addAndMakeVisible(addVstBtn);
    addVstBtn.onClick=[this]{ addVst(); };

    addAndMakeVisible(audioBtn);
    audioBtn.onClick=[this]{ audioSetup(); };

    addAndMakeVisible(guideBtn);
    guideBtn.onClick=[this]{ showGuide(); };

    for (auto* s:{&micVol,&musicVol,&masterVol}) {
        s->setRange(0.0,1.5,0.01); s->setValue(1.0); s->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s->setTextBoxStyle(juce::Slider::TextBoxBelow,false,70,20);
        addAndMakeVisible(*s);
    }

    meters.setText("MIC  0.0 dB     MUSIC  0.0 dB     MASTER  0.0 dB",juce::dontSendNotification);
    meters.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(meters);

    addAndMakeVisible(vstList);
    startTimerHz(30);
    setLookAndFeel(nullptr);
}

MainComponent::~MainComponent(){}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff080b12));
    g.setColour(juce::Colour(0xff182638)); g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(8),8,1.0f);
    g.setColour(juce::Colour(0xff18dcff)); g.drawLine(18,54,getWidth()-18,54,1.5f);
    g.setColour(juce::Colour(0xff131c27)); g.fillRoundedRectangle(18,70,getWidth()-36,140,7);
    g.setColour(juce::Colour(0xff24dcff)); g.drawRoundedRectangle(18,70,getWidth()-36,140,7,1.0f);
    g.setColour(juce::Colour(0xff121a24)); g.fillRoundedRectangle(18,222,getWidth()-36,130,7);
    g.setColour(juce::Colour(0xff273b50)); g.drawRoundedRectangle(18,222,getWidth()-36,130,7,1.0f);
    g.setColour(juce::Colour(0xff111a24)); g.fillRoundedRectangle(18,365,getWidth()-36,110,7);
    g.setColour(juce::Colour(0xff273b50)); g.drawRoundedRectangle(18,365,getWidth()-36,110,7,1.0f);
    g.setColour(juce::Colour(0xff6d8298));
    g.setFont(11.0f);
    g.drawFittedText("MIC VOLUME        NHẠC NỀN        MASTER",35,370,getWidth()-70,20,juce::Justification::centred,false);
    g.drawFittedText("VST RACK — âm thanh đi qua plugin thật theo đúng thứ tự",35,490,getWidth()-70,20,juce::Justification::centred,false);
    g.drawFittedText("Phần mềm được tạo bởi Hoài Nguyễn Studio  •  Zalo: 0965.043.000",35,getHeight()-30,getWidth()-70,18,juce::Justification::centred,false);
}

void MainComponent::resized()
{
    title.setBounds(20,8,getWidth()-40,42);
    live.setBounds(28,80,150,38);
    stereo.setBounds(190,80,120,38);
    audioBtn.setBounds(320,80,110,38);
    guideBtn.setBounds(440,80,115,38);
    addVstBtn.setBounds(getWidth()-155,80,125,38);
    meters.setBounds(35,215,getWidth()-70,28);
    micVol.setBounds(70,390,160,90);
    musicVol.setBounds(445,390,160,90);
    masterVol.setBounds(820,390,160,90);
    vstList.setBounds(35,515,getWidth()-70,120);
}

void MainComponent::timerCallback()
{
    auto db=[](float x){ return x>0.000001f ? 20.0f*std::log10(x) : -120.0f; };
    meters.setText("MIC "+juce::String(db(engine.getMicRms()),1)+" dB     MUSIC "+juce::String(db(engine.getMusicRms()),1)+" dB     MASTER "+juce::String(db(engine.getMasterRms()),1)+" dB",juce::dontSendNotification);
}

void MainComponent::setLive()
{
    engine.setMicVolume((float)micVol.getValue());
    engine.setMusicVolume((float)musicVol.getValue());
    engine.setMasterVolume((float)masterVol.getValue());
    if (live.getToggleState()) {
        engine.start();
        live.setButtonText("🔴 LIVE ON");
    } else live.setButtonText("LIVE OFF");
}

void MainComponent::toggleStereoMix()
{
    engine.setStereoMix(stereo.getToggleState());
    // Stereo Mix only affects the system-audio/loopback source.
    // It never gates the independent internal SFX bus.
}

void MainComponent::addVst()
{
    juce::FileChooser fc("Chọn VST3", {}, "*.vst3");
    if (!fc.browseForFileToOpen()) return;
    juce::String error;
    if (!engine.vstRack().addVst3(fc.getResult(),error))
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,"VST3",error);
}

void MainComponent::audioSetup()
{
    auto* w=new juce::AlertWindow("AUDIO I/O","Thiết bị vào/ra của HNStudio",juce::AlertWindow::NoIcon);
    w->addTextBlock("Mic và nhạc nền là hai đường tín hiệu riêng. Stereo Mix chỉ bật/tắt đường loopback hệ thống; âm thanh hiệu ứng/SFX của HNStudio vẫn đi qua SFX bus.");
    w->addTextBlock("Chọn thiết bị trong Windows/Audio Device Settings để sử dụng interface hoặc sound card. ASIO/VST3 host là phần engine realtime.");
    w->addButton("ĐÓNG",1);
    w->enterModalState(true,juce::ModalCallbackFunction::create([w](int){ delete w; }));
}

void MainComponent::showGuide()
{
    auto* w=new juce::AlertWindow("HƯỚNG DẪN HNSTUDIO MUSIK AI",
        "1. Chọn MIC và OUTPUT trong Audio Setup.\n"
        "2. Bật LIVE để mở đường microphone.\n"
        "3. MIC, NHẠC NỀN và MASTER có volume riêng.\n"
        "4. Stereo Mix chỉ dành cho loopback hệ thống; tắt nó không làm mất SFX nội bộ.\n"
        "5. ADD VST để nạp VST3 thật vào VST Rack.\n"
        "6. Mỗi VST có bypass và xử lý theo thứ tự trong chain.\n"
        "7. Auto-Tune HN và VST Auto-Tune là hai lớp độc lập.\n"
        "8. Lưu Project để lưu cấu hình.",juce::AlertWindow::InfoIcon);
    w->addButton("ĐÓNG",1);
    w->enterModalState(true,juce::ModalCallbackFunction::create([w](int){ delete w; }));
}
