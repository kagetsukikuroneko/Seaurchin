#pragma once

#include "Debug.h"
#include "Font.h"
#include "EffectData.h"
#include "SoundManager.h"
#include "Misc.h"

#define SU_IF_IMAGE "Image"
#define SU_IF_FONT "Font"
#define SU_IF_RENDER "RenderTarget"
#define SU_IF_SOUND "Sound"
#define SU_IF_EFXDATA "EffectData"

//interface 自動解放対象
class ISResouceAutoRelease
{
    virtual bool Dispose() = 0;
};

//リソース基底クラス
class SResource
{
protected:
    int Reference = 0;
    int Handle = 0;
public:
    SResource();
    virtual ~SResource();
    void AddRef();
    void Release();

    inline int GetHandle() { return Handle; }
};

//画像
class SImage : public SResource
{
protected:
    int Width = 0;
    int Height = 0;

    void ObtainSize();
public:
    SImage(int ih);
    ~SImage() override;

    int get_Width();
    int get_Height();

    static SImage* CreateBlankImage();
    static SImage* CreateLoadedImageFromFile(const std::string &file);
    static SImage* CreateLoadedImageFromMemory(void *buffer, size_t size);
};

//描画タゲ
class SRenderTarget : public SImage
{
public:
    SRenderTarget(int w, int h);

    static SRenderTarget* CreateBlankTarget(int w, int h);
};

//フォント
class SFont : public SResource
{
protected:
    int Size = 0;
    std::vector<GlyphInfo*> Chars;

public:
    std::vector<SImage*> Images;
    SFont();
    ~SFont() override;

    inline int get_Size() { return Size; }
    std::tuple<double, double, int> RenderRaw(SRenderTarget *rt, const std::wstring& str);

    static SFont* CreateBlankFont();
    static SFont* CreateLoadedFontFromFile(const std::string &file);
};

//エフェクト
class SEffect : public SResource
{
protected:
    
public:
    EffectData *data;

    SEffect(EffectData *rawdata);
    ~SEffect() override;
};

class SSound : public SResource {
protected:
	SoundManager *manager = nullptr;
	SoundSample *sample;

public:
    SSound(SoundManager *mng, SoundSample *smp);
    ~SSound() override;

    void Play();
    void StopAll();

    static SSound* CreateSound(SoundManager *smanager);
    static SSound* CreateSoundFromFile(SoundManager *smanager, const std::string &file, int simul);
};

class ExecutionManager;
void RegisterScriptResource(ExecutionManager *exm);