//
//  Crystal Text-to-Speech Engine
//
//  Copyright (c) 2007 THU-CUHK Joint Research Center for
//  Media Sciences, Technologies and Systems. All rights reserved.
//
//  http://mjrc.sz.tsinghua.edu.cn
//
//  Redistribution and use in source and binary forms, with or without
//  modification, is not allowed, unless a valid written license is
//  granted by THU-CUHK Joint Research Center.
//
//  THU-CUHK Joint Research Center has the rights to create, modify,
//  copy, compile, remove, rename, explain and deliver the source codes.
//

///
/// @file
///
/// @brief  Head file for the interface of speech library accessing
///
/// <b>History:</b>
/// - Version:  0.1.0
///   Author:   John (john.zywu@gmail.com)
///   Date:     2007/06/15
///   Changed:  Created
/// - Version:  0.2.0
///   Author:   John (john.zywu@gmail.com)
///   Date:     2008/07/30
///   Changed:  Modified by using the new definitions
///


#ifndef _CST_TTS_BASE_SPLIB_SPEECHLIB_H_
#define _CST_TTS_BASE_SPLIB_SPEECHLIB_H_

#include "cmn/cmn_type.h"
#include <string>
#include <vector>

namespace cst
{
    namespace tts
    {
        namespace base
        {
            ///
            /// @brief  The base class for context label information of speech unit in speech library.
            ///         It can be inherited for different engines and modules.
            ///
            class CContextLabel
            {
            public:
                virtual ~CContextLabel() {} ///< Only the object with virtual tables can use dynamic_cast
            };

            ///
            /// @brief  The base class for prosody tag information of speech unit in speech library.
            ///         It can be inherited for different engines and modules
            ///
            class CProsodyTag
            {
            public:
                virtual ~CProsodyTag() {}   ///< Only the object with virtual tables can use dynamic_cast
            };

            ///
            /// @brief  The base implementation interface class for speech library accessing
            ///
            class CSpeechLib
            {
            public:
                ///
                /// @brief  The detailed description of the speech library
                ///
                /// The languages and the accents must appear in pairs, which means each language must have one accent.
                /// If there is no (obvious) accent for one language, the accent must be set to "" (empty string).
                /// For example, "zh-cmn:zh-HK" means the speaker can speak Chinese Putonghua but with Hong Kong accent.
                /// "zh-yue" means the speaker can speak Chinese Cantonese (does not care about the accent).
                ///
                class CDescriptor
                {
                public:
                    wchar_t         chGender;       ///< The gender of the speaker for the speech library
                    int             nAge;           ///< The age in years (since birth) of the speaker
                    int             nVariant;       ///< The variant of the other characters of the speaker (e.g. the second male child voice)
                    std::wstring    wstrName;       ///< The name of the speaker
                    std::vector<std::wstring> vecLanguage;  ///< The languages which the speaker can speak
                    std::vector<std::wstring> vecAccent;    ///< The accents of the speaker, corresponding to languages one by one
                };

            public:
                //////////////////////////////////////////////////////////////////////////
                //
                //  Initialization and finalization
                //
                //////////////////////////////////////////////////////////////////////////

                ///
                /// @brief  Constructor
                ///
                CSpeechLib() : m_bInitialized(false) {}

                ///
                /// @brief  Destructor
                ///
                virtual ~CSpeechLib() {m_bInitialized = false;}

                ///
                /// @brief  Initialize the speech library module
                ///
                /// @param  [in] wstrPath   The path name where speech library data are stored
                ///
                /// @return Whether speech library is initialized successfully
                ///
                virtual bool initialize(const std::wstring &wstrPath) = 0;

                ///
                /// @brief  Terminate and free the speech library module, close all the data
                ///
                /// @return Whether speech library is terminated successfully
                ///
                virtual bool terminate() = 0;

                ///
                /// @brief  Indicating whether speech library is initialized
                ///
                bool isReady() const {return m_bInitialized;}

            public:
                //////////////////////////////////////////////////////////////////////////
                //
                //  Common data manipulation (overrides)
                //
                //////////////////////////////////////////////////////////////////////////

                ///
                /// @brief  Get internal code of the input phoneme.
                ///         The internal code will be used for all the data manipulation interfaces.
                ///
                /// @param  [in] wstrPhoneme    The input phoneme whose internal code is to be returned
                ///
                /// @return The internal code of the input phoneme
                /// @retval INVALID_ICODE(-1) The input phoneme is not valid or supported
                ///
                virtual icode_t getICodeFromPhoneme(const std::wstring &wstrPhoneme) const = 0;

                ///
                /// @brief  Get the corresponding phoneme of the input internal code.
                ///
                /// @param  [in] iCode  The internal code whose corresponding phoneme is to be returned
                ///
                /// @return The phoneme of the input internal code
                /// @retval ""(empty string)  The input internal code is not valid or supported
                ///
                virtual std::wstring getPhonemeFromICode(icode_t iCode) const = 0;

                ///
                /// @brief  Get the number of phonemes in the current speech library. 
                ///         This number corresponds to the maximum internal code.
                ///
                /// @return The number (the maximum internal code) of the phonemes
                ///
                virtual uint32 getPhonemeNumber() const = 0;

                ///
                /// @brief  Get the total number of candidate speech units in the speech library of the input phoneme (iCode)
                ///
                /// @param  [in]  iCode     The internal code of the phoneme
                ///
                /// @return The total number of speech units in the library of the specified phoneme (iCode)
                ///
                virtual uint32 getUnitNumber(icode_t iCode) const = 0;

                ///
                /// @brief  Get all the context label data for the input phoneme (iCode), the labels are returned in array
                ///
                /// @param  [in]  iCode     The internal code of the phoneme
                /// @param  [in]  labelNum  The maximum number of labels to be retrieved
                /// @param  [out] labelNum  Return the actual number of retrieved labels (valid number in returned array)
                /// @param  [out] retLabels Return the retrieved context labels (in array)
                ///
                /// @return Whether operation is successful
                ///
                virtual bool getContextLabels(icode_t iCode, CContextLabel *retLabels, uint32 &labelNum) const = 0;

                ///
                /// @brief  Get one context label information for the specific speech unit of the input phoneme (iCode)
                ///
                /// @param  [in]  iCode     The internal code of the phoneme
                /// @param  [in]  nIndex    The index position of the speech unit
                /// @param  [out] retLabel  Return the retrieved context label (only one)
                ///
                /// @return Whether operation is successful
                ///
                virtual bool getContextLabel(icode_t iCode, uint32 nIndex, CContextLabel &retLabel) const = 0;

                ///
                /// @brief  Get one prosodic tag information for the specific speech unit of the input phoneme (iCode)
                ///
                /// @param  [in]  iCode     The internal code of the phoneme
                /// @param  [in]  nIndex    The index position of the speech unit
                /// @param  [out] retTag    Return the retrieved prosodic tag information (only one)
                ///
                /// @return Whether operation is successful
                ///
                virtual bool getProsodyTag(icode_t iCode, uint32 nIndex, CProsodyTag &retTag) const = 0;

                ///
                /// @brief  Get the wave data for the specific speech unit of the input phoneme (iCode)
                ///
                /// @param  [in]  iCode     The internal code of the phoneme
                /// @param  [in]  nIndex    The index position of the speech unit
                /// @param  [in]  waveLen   The maximum buffer length to retrieve the wave data (Unit: uint8/byte)
                /// @param  [out] waveLen   Return the actual wave length of the retrieved wave data (unit: uint8/byte)
                /// @param  [out] waveData  Return the retrieved wave data
                ///
                /// @return Whether operation is successful
                ///
                virtual bool getWave(icode_t iCode, uint32 nIndex, uint8 *waveData, uint32 &waveLen) const = 0;

                ///
                /// @brief  Get the wave length for the specific speech unit of the input phoneme (iCode)
                ///
                /// @param  [in]  iCode     The internal code of the phoneme
                /// @param  [in]  nIndex    The index position of the speech unit
                ///
                /// @return The wave length of the specified speech unit (Unit: uint8/byte)
                ///
                virtual uint32 getWaveLength(icode_t iCode, uint32 nIndex) const = 0;

            public:
                //////////////////////////////////////////////////////////////////////////
                //
                //  Speech library general information manipulation
                //
                //////////////////////////////////////////////////////////////////////////

                ///
                /// @brief  Get the detailed description of the speech library
                ///
                virtual const CDescriptor &getDescriptor() const {return m_descriptor;}

                ///
                /// @brief  Get the wave sampling rate (samples per second)
                ///
                virtual int getSamplesPerSec() const = 0;

                ///
                /// @brief  Get the wave sampling precision (bits per sample)
                ///
                virtual int getBitsPerSample() const = 0;

                ///
                /// @brief  Get the wave channel number
                ///
                virtual int getChannels() const = 0;

            protected:
                bool m_bInitialized;        ///< Whether the speech library is initialized or not
                CDescriptor m_descriptor;   ///< The detailed description of the speech library

            private:
                ///
                /// @brief  Declared to prevent calling and implementation
                ///
                CSpeechLib &operator = (const CSpeechLib &);
                ///
                /// @brief  Declared to prevent calling and implementation
                ///
                CSpeechLib(const CSpeechLib &);
            };

        }//namespace base
    }
}

#endif//_CST_TTS_BASE_SPLIB_SPEECHLIB_H_
