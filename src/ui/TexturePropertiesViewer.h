#pragma once
#include "mycommon.h"
#include "metro/MetroTexturesDatabase.h"
#include "metro/MetroTexture.h"

#include "ui/UIHelpers.h"

namespace MetroEX {
    using namespace System;
    using namespace msclr::interop;
    using namespace System::ComponentModel;

    ref class TexturePropertiesViewer {
    public:
        TexturePropertiesViewer() {
            mAuxNames = gcnew array<String^>(8);
        }
        ~TexturePropertiesViewer() {
            MySafeDelete(mAuxNames);
        }

    private:
        const MetroTextureInfo* mTextureInfo;
        String^ mRealPath;
        array<String^>^ mAuxNames;

    public:
        void SetTextureInfo(const MetroTextureInfo* textureInfo) {
            mTextureInfo = textureInfo;

            if (mTextureInfo) {
                mAuxNames[0] = ToNetString(mTextureInfo->aux0_name);
                mAuxNames[1] = ToNetString(mTextureInfo->aux1_name);
                mAuxNames[2] = ToNetString(mTextureInfo->aux2_name);
                mAuxNames[3] = ToNetString(mTextureInfo->aux3_name);
                mAuxNames[4] = ToNetString(mTextureInfo->aux4_name);
                mAuxNames[5] = ToNetString(mTextureInfo->aux5_name);
                mAuxNames[6] = ToNetString(mTextureInfo->aux6_name);
                mAuxNames[7] = ToNetString(mTextureInfo->aux7_name);
            }
        }

        void SetRealPath(String^ path) {
            mRealPath = path;
        }

        [Category("Common")]
        [Description("Texture name")]
        property String^ Name {
            String^ get() { return ToNetString(mTextureInfo->name); }
        };

        [Category("Common")]
        [Description("Texture type")]
        property String^ Type {
            String^ get() {
                switch (safe_cast<MetroTextureInfo::TextureType>(mTextureInfo->type)) {
                case MetroTextureInfo::TextureType::Diffuse:
                    return "Diffuse";
                case MetroTextureInfo::TextureType::Detail_diffuse:
                    return "Detail_diffuse";
                case MetroTextureInfo::TextureType::Cubemap:
                    return "Cubemap";
                case MetroTextureInfo::TextureType::Cubemap_hdr:
                    return "Cubemap_hdr";
                case MetroTextureInfo::TextureType::Terrain:
                    return "Terrain";
                case MetroTextureInfo::TextureType::Bumpmap:
                    return "Bumpmap";
                case MetroTextureInfo::TextureType::Diffuse_va:
                    return "Diffuse_va";
                case MetroTextureInfo::TextureType::Arbitrary4:
                    return "Arbitrary4";
                case MetroTextureInfo::TextureType::Normalmap:
                    return "Normalmap";
                case MetroTextureInfo::TextureType::Normalmap_alpha:
                    return "Normalmap_alpha";
                case MetroTextureInfo::TextureType::Normalmap_detail:
                    return "Normalmap_detail";
                case MetroTextureInfo::TextureType::Unknown_01:
                    return "Unknown_01";
                case MetroTextureInfo::TextureType::Unknown_has_lum:
                    return "Unknown_has_lum";
                case MetroTextureInfo::TextureType::Instance:
                    return "Instance";
                default:
                    return "Unknown";
                }
            }
        }

        [Category("Common")]
        [Description("Texture source name")]
        property String^ SourceName {
            String^ get() {
                return ToNetString(mTextureInfo->source_name);
            }
        }

        property vec4 SurfaceXForm {
            vec4 get() { return mTextureInfo->surf_xform; }
        }

        [Category("Common")]
        [Description("Texture format")]
        property String^ Format {
            String^ get() {
                switch (safe_cast<MetroTextureInfo::PixelFormat>(mTextureInfo->format)) {
                case MetroTextureInfo::PixelFormat::Invalid:
                    return "Invalid";
                case MetroTextureInfo::PixelFormat::BC1:
                    return "BC1";
                case MetroTextureInfo::PixelFormat::BC3:
                    return "BC3";
                case MetroTextureInfo::PixelFormat::RGBA8_UNORM:
                    return "RGBA8_UNORM";
                case MetroTextureInfo::PixelFormat::RGBA8_SNORM:
                    return "RGBA8_SNORM";
                case MetroTextureInfo::PixelFormat::BC7:
                    return "BC7";
                case MetroTextureInfo::PixelFormat::RG8_UNORM:
                    return "RG8_UNORM";
                case MetroTextureInfo::PixelFormat::RG8_SNORM:
                    return "RG8_SNORM";
                case MetroTextureInfo::PixelFormat::DEPTH_32F_S8:
                    return "DEPTH_32F_S8";
                case MetroTextureInfo::PixelFormat::DEPTH_32F:
                    return "DEPTH_32F";
                case MetroTextureInfo::PixelFormat::R32_F:
                    return "R32_F";
                case MetroTextureInfo::PixelFormat::RGBA16_F:
                    return "RGBA16_F";
                case MetroTextureInfo::PixelFormat::RG16_F:
                    return "RG16_F";
                case MetroTextureInfo::PixelFormat::RGBA16_U:
                    return "RGBA16_U";
                case MetroTextureInfo::PixelFormat::R8_UNORM:
                    return "R8_UNORM";
                case MetroTextureInfo::PixelFormat::R8_U:
                    return "R8_U";
                case MetroTextureInfo::PixelFormat::RGB10_UNORM_A2_UNORM:
                    return "RGB10_UNORM_A2_UNORM";
                case MetroTextureInfo::PixelFormat::RGB10_SNORM_A2_UNORM:
                    return "RGB10_SNORM_A2_UNORM";
                case MetroTextureInfo::PixelFormat::R11G11B10_F:
                    return "R11G11B10_F";
                case MetroTextureInfo::PixelFormat::R16_UNORM:
                    return "R16_UNORM";
                case MetroTextureInfo::PixelFormat::R32_U:
                    return "R32_U";
                case MetroTextureInfo::PixelFormat::RGBA32_F:
                    return "RGBA32_F";
                case MetroTextureInfo::PixelFormat::PPS:
                    return "PPS";
                case MetroTextureInfo::PixelFormat::BGRA8_UNORM:
                    return "BGRA8_UNORM";
                default:
                    return "Unknown";
                }
            }
        }

        [Category("Size")]
        [Description("Texture Width in pixels")]
        property uint32_t Width {
            uint32_t get() { return mTextureInfo->width; }
        }

        [Category("Size")]
        [Description("Texture Height in pixels")]
        property uint32_t Height {
            uint32_t get() { return mTextureInfo->height; }
        };

        property bool Animated {
            bool get() { return mTextureInfo->animated; }
        };

        [Category("Common")]
        [Description("Texture real path")]
        property String^ RealPath {
            String^ get() { return mRealPath; }
        };

    #pragma region Material properties

        [Category("Material")]
        [Description("Shader name")]
        property String^ ShaderName {
            String^ get() { return ToNetString(mTextureInfo->shader_name); }
        };

        [Category("Material")]
        [Description("Bump Name")]
        property String^ BumpName {
            String^ get() { return ToNetString(mTextureInfo->bump_name); }
        };

        [Category("Material")]
        [Description("Bump height")]
        property float BumpHeight {
            float get() { return mTextureInfo->bump_height; }
        };

        [Category("Material")]
        [Description("Parallax height multiplicator")]
        property float ParallaxHeightMult {
            float get() { return mTextureInfo->parallax_height_mul; }
        };

        [Category("Material")]
        [Description("Reflectivity factor")]
        property float Reflectivity {
            float get() { return mTextureInfo->reflectivity; }
        };

        [Category("Material")]
        [Description("Is metal")]
        property bool IsMetal {
            bool get() { return mTextureInfo->treat_as_metal; }
        };

        [Category("Material")]
        [Description("Detail texture name")]
        property String^ DetailName {
            String^ get() { return ToNetString(mTextureInfo->det_name); }
        };

        [Category("Material")]
        [Description("Detail texture scale")]
        property System::Drawing::PointF DetailScale {
            System::Drawing::PointF get() { return System::Drawing::PointF(mTextureInfo->det_scale_u, mTextureInfo->det_scale_v); }
        };

        [Category("Material")]
        [Description("Detail texture intensity")]
        property float DetailIntensity {
            float get() { return mTextureInfo->det_intensity; }
        };

        [Category("Material")]
        [Description("Auxiliary texture names")]
        property array<String^>^ AuxNames {
            array<String^>^ get() {
                return mAuxNames;
            }
        };

    #pragma endregion
    };
}
