﻿/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2025, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

/**
 *  @file
 *  @brief Default file I/O using fXXX()-family of functions
 */
#pragma once
#ifndef AI_DEFAULTIOSTREAM_H_INC
#define AI_DEFAULTIOSTREAM_H_INC

#ifdef __GNUC__
#   pragma GCC system_header
#endif

#include <cstdio>
#include <assimp/IOStream.hpp>
#include <assimp/importerdesc.h>

namespace Assimp {

// ----------------------------------------------------------------------------------
//! @class  DefaultIOStream
//! @brief  Default IO implementation, use standard IO operations
//! @note   An instance of this class can exist without a valid file handle
//!         attached to it. All calls fail, but the instance can nevertheless be
//!         used with no restrictions.
class ASSIMP_API DefaultIOStream : public IOStream {
    friend class DefaultIOSystem;
#if __ANDROID__
# if __ANDROID_API__ > 9
#  if defined(AI_CONFIG_ANDROID_JNI_ASSIMP_MANAGER_SUPPORT)
    friend class AndroidJNIIOSystem;
#  endif // defined(AI_CONFIG_ANDROID_JNI_ASSIMP_MANAGER_SUPPORT)
# endif // __ANDROID_API__ > 9
#endif // __ANDROID__

protected:
    /// @brief
    DefaultIOStream() AI_NO_EXCEPT;

    /// @brief The class constructor with the file name and the stream.
    /// @param pFile        The file-streaam
    /// @param strFilename  The file name
    DefaultIOStream(FILE* pFile, const std::string &strFilename);

public:
    /** Destructor public to allow simple deletion to close the file. */
    ~DefaultIOStream () override;

    // -------------------------------------------------------------------
    /// Read from stream
    size_t Read(void* pvBuffer, size_t pSize, size_t pCount) override;

    // -------------------------------------------------------------------
    /// Write to stream
    size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) override;

    // -------------------------------------------------------------------
    /// Seek specific position
    aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;

    // -------------------------------------------------------------------
    /// Get current seek position
    size_t Tell() const override;

    // -------------------------------------------------------------------
    /// Get size of file
    size_t FileSize() const override;

    // -------------------------------------------------------------------
    /// Flush file contents
    void Flush() override;

private:
    FILE* mFile;
    std::string mFilename;
    mutable size_t mCachedSize;
};

// ----------------------------------------------------------------------------------
AI_FORCE_INLINE DefaultIOStream::DefaultIOStream() AI_NO_EXCEPT :
        mFile(nullptr),
        mFilename(),
        mCachedSize(SIZE_MAX) {
    // empty
}

// ----------------------------------------------------------------------------------
AI_FORCE_INLINE DefaultIOStream::DefaultIOStream (FILE* pFile, const std::string &strFilename) :
        mFile(pFile),
        mFilename(strFilename),
        mCachedSize(SIZE_MAX) {
    // empty
}

// ----------------------------------------------------------------------------------

} // ns assimp

#endif //!!AI_DEFAULTIOSTREAM_H_INC
