/*
Copyright(c) 2016 Christopher Joseph Dean Schaefer

This software is provided 'as-is', without any express or implied
warranty.In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software.If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
 
#include "CVertexBuffer9.h"
#include "CDevice9.h"

#include "Utilities.h"

CVertexBuffer9::CVertexBuffer9(CDevice9* device, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, HANDLE* pSharedHandle)
	: mReferenceCount(1),
	mDevice(device),
	mLength(Length),
	mUsage(Usage),
	mFVF(FVF),
	mPool(Pool),
	mSharedHandle(pSharedHandle),
	mSize(0),
	mCapacity(0),
	mIsDirty(true),
	mLockCount(0)
{
	mCommandStreamManager = device->mCommandStreamManager;
	WorkItem* workItem = mCommandStreamManager->GetWorkItem();
	workItem->Id = device->mId;
	workItem->WorkItemType = WorkItemType::VertexBuffer_Create;
	workItem->Argument1 = this;
	mId = mCommandStreamManager->RequestWork(workItem);
}

CVertexBuffer9::~CVertexBuffer9()
{	
	WorkItem* workItem = mCommandStreamManager->GetWorkItem();
	workItem->WorkItemType = WorkItemType::VertexBuffer_Destroy;
	workItem->Id = mId;
	mCommandStreamManager->RequestWork(workItem);	
}

ULONG STDMETHODCALLTYPE CVertexBuffer9::AddRef(void)
{
	return InterlockedIncrement(&mReferenceCount);
}

HRESULT STDMETHODCALLTYPE CVertexBuffer9::QueryInterface(REFIID riid,void  **ppv)
{
	if (ppv == nullptr)
	{
		return E_POINTER;
	}

	if (IsEqualGUID(riid, IID_IDirect3DVertexBuffer9))
	{
		(*ppv) = this;
		this->AddRef();
		return S_OK;
	}

	if (IsEqualGUID(riid, IID_IDirect3DResource9))
	{
		(*ppv) = this;
		this->AddRef();
		return S_OK;
	}

	if (IsEqualGUID(riid, IID_IUnknown))
	{
		(*ppv) = this;
		this->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CVertexBuffer9::Release(void)
{
	ULONG ref = InterlockedDecrement(&mReferenceCount);

	if (ref == 0)
	{
		delete this;
	}

	return ref;
}

HRESULT STDMETHODCALLTYPE CVertexBuffer9::GetDevice(IDirect3DDevice9** ppDevice)
{ 
	mDevice->AddRef(); 
	(*ppDevice) = (IDirect3DDevice9*)mDevice; 
	return S_OK; 
}

HRESULT STDMETHODCALLTYPE CVertexBuffer9::FreePrivateData(REFGUID refguid)
{
	//TODO: Implement.

	BOOST_LOG_TRIVIAL(warning) << "CVertexBuffer9::FreePrivateData is not implemented!";

	return E_NOTIMPL;
}

DWORD STDMETHODCALLTYPE CVertexBuffer9::GetPriority()
{
	//TODO: Implement.

	BOOST_LOG_TRIVIAL(warning) << "CVertexBuffer9::GetPriority is not implemented!";

	return 1;
}

HRESULT STDMETHODCALLTYPE CVertexBuffer9::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	//TODO: Implement.

	BOOST_LOG_TRIVIAL(warning) << "CVertexBuffer9::GetPrivateData is not implemented!";

	return E_NOTIMPL;
}

D3DRESOURCETYPE STDMETHODCALLTYPE CVertexBuffer9::GetType()
{
	//return D3DRTYPE_SURFACE;
	//return D3DRTYPE_VOLUME;
	//return D3DRTYPE_TEXTURE;
	//return D3DRTYPE_VOLUMETEXTURE;
	//return D3DRTYPE_CUBETEXTURE;
	return D3DRTYPE_VERTEXBUFFER;
	//return D3DRTYPE_INDEXBUFFER;
}

void STDMETHODCALLTYPE CVertexBuffer9::PreLoad()
{
	//TODO: Implement.

	BOOST_LOG_TRIVIAL(warning) << "CVertexBuffer9::PreLoad is not implemented!";

	return; 
}

DWORD STDMETHODCALLTYPE CVertexBuffer9::SetPriority(DWORD PriorityNew)
{
	//TODO: Implement.

	BOOST_LOG_TRIVIAL(warning) << "CVertexBuffer9::SetPriority is not implemented!";

	return 1;
}

HRESULT STDMETHODCALLTYPE CVertexBuffer9::SetPrivateData(REFGUID refguid, const void* pData, DWORD SizeOfData, DWORD Flags)
{
	//TODO: Implement.

	BOOST_LOG_TRIVIAL(warning) << "CVertexBuffer9::SetPrivateData is not implemented!";

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CVertexBuffer9::GetDesc(D3DVERTEXBUFFER_DESC* pDesc)
{
	//TODO: Implement.

	BOOST_LOG_TRIVIAL(warning) << "CVertexBuffer9::GetDesc is not implemented!";

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CVertexBuffer9::Lock(UINT OffsetToLock, UINT SizeToLock, VOID** ppbData, DWORD Flags)
{
	if (mPool == D3DPOOL_MANAGED)
	{
		if (!(Flags & D3DLOCK_READONLY))
		{ //If the lock allows write mark the buffer as dirty.
			mIsDirty = true;
		}
	}

	InterlockedIncrement(&mLockCount);

	WorkItem* workItem = mCommandStreamManager->GetWorkItem();
	workItem->WorkItemType = WorkItemType::VertexBuffer_Lock;
	workItem->Id = mId;
	workItem->Argument1 = OffsetToLock;
	workItem->Argument2 = SizeToLock;
	workItem->Argument3 = ppbData;
	workItem->Argument4 = Flags;
	mCommandStreamManager->RequestWorkAndWait(workItem);

	return S_OK;	
}

HRESULT STDMETHODCALLTYPE CVertexBuffer9::Unlock()
{
	WorkItem* workItem = mCommandStreamManager->GetWorkItem();
	workItem->WorkItemType = WorkItemType::VertexBuffer_Unlock;
	workItem->Id = mId;
	mCommandStreamManager->RequestWork(workItem);

	InterlockedDecrement(&mLockCount);

	return S_OK;	
}