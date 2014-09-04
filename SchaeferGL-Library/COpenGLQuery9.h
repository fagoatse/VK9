//========= Copyright Valve Corporation, All rights reserved. ============//
//                       TOGL CODE LICENSE
//
//  Copyright 2011-2014 Valve Corporation
//  All Rights Reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
// dxabstract.h
//
//==================================================================================================

/*
 * Code from ToGL has been modified to fit the design.
 */
 
#ifndef COPENGLQUERY9_H
#define COPENGLQUERY9_H

#include "IDirect3DQuery9.h" // Base class: IDirect3DQuery9
#include "COpenGLResource9.h"

class COpenGLQuery9 : public IDirect3DQuery9,COpenGLResource9
{
public:
	COpenGLQuery9();
	~COpenGLQuery9();

	D3DQUERYTYPE			m_type;		// D3DQUERYTYPE_OCCLUSION or D3DQUERYTYPE_EVENT
	GLMContext				*m_ctx;
	CGLMQuery				*m_query;
	
	uint					m_nIssueStartThreadID, m_nIssueEndThreadID;
	uint					m_nIssueStartDrawCallIndex, m_nIssueEndDrawCallIndex;
	uint					m_nIssueStartFrameIndex, m_nIssueEndFrameIndex;
	uint					m_nIssueStartQueryCreationCounter, m_nIssueEndQueryCreationCounter;

public:
	virtual HRESULT GetData(void* pData, DWORD dwSize, DWORD dwGetDataFlags);
	//virtual DWORD GetDataSize();
	v//irtual HRESULT GetDevice(IDirect3DDevice9** pDevice);
	//virtual D3DQUERYTYPE GetType();
	virtual HRESULT Issue(DWORD dwIssueFlags);
};

#endif // COPENGLQUERY9_H