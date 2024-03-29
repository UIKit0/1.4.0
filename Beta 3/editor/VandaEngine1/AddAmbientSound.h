//Copyright (C) 2015 Zehne Ziba Co., Ltd.
//This file is licensed and distributed under MIT license

#pragma once
#include "afxwin.h"


// CAddAmbientSound dialog

class CAddAmbientSound : public CDialog
{
	DECLARE_DYNAMIC(CAddAmbientSound)

public:
	CAddAmbientSound(CWnd* pParent = NULL)
		;   // standard constructor
	virtual ~CAddAmbientSound();

// Dialog Data
	enum { IDD = IDD_DIALOG_ADD_AMBIENT_SOUND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	CChar m_name[MAX_NAME_SIZE];
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAmbientSound();
public:
	CEdit m_editBoxAmbientSoundBuffer;
	CString m_strAmbientSoundBuffer, m_strPureAmbientSoundBuffer, m_strAmbientSoundName, m_strAmbientSoundVolume, m_strAmbientSoundPitch;
	CString m_strAmbientSoundTempName;
	CFloat m_volume, m_pitch;
	CBool m_create;
	CBool m_editMode;
public:
	CEdit m_editBoxAmbientSoundName;
public:
	afx_msg void OnEnChangeEditAmbientSoundName();

	//####public interface####	
	//The user just needs to use these functions

	CChar* GetName() { return (CChar*)m_strAmbientSoundName.GetBuffer(m_strAmbientSoundName.GetLength()); m_strAmbientSoundName.ReleaseBuffer(); }
	CChar* GetTempName() { return (CChar*)m_strAmbientSoundTempName.GetBuffer(m_strAmbientSoundTempName.GetLength()); m_strAmbientSoundTempName.ReleaseBuffer(); }
	CChar* GetPath() { return (CChar*)m_strAmbientSoundBuffer.GetBuffer(m_strAmbientSoundBuffer.GetLength()); m_strAmbientSoundBuffer.ReleaseBuffer(); }
	CFloat GetVolume() { return m_volume; }
	CFloat GetPitch() { return m_pitch; }

	CVoid SetEditMode( CBool editMode )
	{
		m_editMode = editMode;
	}
	CVoid SetCreate( CBool create )
	{
		if( create ) m_create = CTrue;
		else m_create = CFalse;
	}
	CVoid SetName( CChar* name )
	{
		m_strAmbientSoundName = name;
	}

	CVoid SetPath( CChar * path )
	{
		m_strAmbientSoundBuffer = path;
	}

	CVoid SetPitch( CFloat pitch )
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf( temp, "%f", pitch );
		m_strAmbientSoundPitch = temp;
	}

	CVoid SetVolume( CFloat volume )
	{
		CChar temp[MAX_NAME_SIZE];
		sprintf( temp, "%f", volume );
		m_strAmbientSoundVolume = temp;
	}
	//#######################

public:
	CEdit m_editBoxAmbientSoundPitch;
public:
	CEdit m_editBoxAmbientSoundVolume;
public:
	afx_msg void OnEnChangeEditAmbientSoundPitch();
public:
	afx_msg void OnEnChangeEditAmbientSoundVolume();
public:
	virtual BOOL OnInitDialog();
public:
	virtual INT_PTR DoModal();
};
