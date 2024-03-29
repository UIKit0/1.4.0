//Copyright (C) 2015 Zehne Ziba Co., Ltd.
//This file is licensed and distributed under MIT license

#pragma once
#include "../common/utility.h"
#include "OpenALSoundSource.h"
#include "OpenALSoundBuffer.h"

class CAmbientSound
{
public:
	CAmbientSound() { m_buffer = NULL; m_source = NULL; }
	~CAmbientSound() { m_source->GetSource();alSourcei(m_source->GetSource(), AL_BUFFER, AL_NONE); CDelete( m_buffer ); CDelete( m_source ) }
	CVoid SetName( CChar* name ) {Cpy( m_name, name ); }
	CVoid SetPath( CChar* path ) {Cpy( m_path, path ); }
	CVoid SetVolume( CFloat volume) { m_volume = volume; }
	CVoid SetPitch( CFloat pitch ) { m_pitch = pitch; }
	CVoid SetSoundSource( COpenALSoundSource* source ) { m_source = source; }
	CVoid SetSoundBuffer( COpenALSoundBuffer* buffer ) { m_buffer = buffer; }
	COpenALSoundSource* GetSoundSource() { return m_source; }
	COpenALSoundBuffer* GetSoundBuffer() { return m_buffer; }

	CChar* GetName(){ return m_name; }
	CChar* GetPath(){ return m_path; }
	CFloat GetVolume() { return m_volume; }
	CFloat GetPitch() { return m_pitch; }
	CChar m_name[MAX_NAME_SIZE];
	CChar m_path[MAX_NAME_SIZE];
	CFloat m_pitch;
	CFloat m_volume;
	COpenALSoundBuffer* m_buffer;
	COpenALSoundSource* m_source;
};
