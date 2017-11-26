// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include <UIRibbon.h>
#include "CommandHandler.h"
#include "RibbonIDs.h"
#include "RibbonFramework.h"
#include <commdlg.h> 
#pragma comment(lib, "ComCtl32.lib")
#include <vector>
#include "lastTest.h"
#include <UIRibbonPropertyHelpers.h>

using namespace std;

//hỗ trợ đọc lưu file
TCHAR sysFile[100];
DWORD NumOfByteWritten;

//tọa độ điểm bắt đầu
extern int firstX;
extern int firstY;
//Tọa độ điểm kết thúc
extern int lastX;
extern int lastY;

extern vector<CShape*> Shapes;
extern  CShape* curShape;

extern HWND g_hwnd;

extern int selectedShape;// kiểu vẽ đang được lựa chọn
extern CStyle StyleForSelection; // style của bút brush đang được chọn


								 // Static method to create an instance of the object.
HRESULT CCommandHandler::CreateInstance(CCommandHandler **ppCommandHandler)
{
	if (!ppCommandHandler)
	{
		return E_POINTER;
	}

	*ppCommandHandler = NULL;

	HRESULT hr = S_OK;

	CCommandHandler* pCommandHandler = new CCommandHandler();

	if (pCommandHandler != NULL)
	{
		*ppCommandHandler = pCommandHandler;
	}
	else
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}

// IUnknown method implementations.
STDMETHODIMP_(ULONG) CCommandHandler::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CCommandHandler::Release()
{
	LONG cRef = InterlockedDecrement(&m_cRef);
	if (cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP CCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(IUICommandHandler))
	{
		*ppv = static_cast<IUICommandHandler*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

//
//  FUNCTION: UpdateProperty()
//
//  PURPOSE: Called by the Ribbon framework when a command property (PKEY) needs to be updated.
//
//  COMMENTS:
//
//    This function is used to provide new command property values, such as labels, icons, or
//    tooltip information, when requested by the Ribbon framework.  
//    
//    In this SimpleRibbon sample, the method is not implemented.  
//

int GetContextMapIDFromCommandID(int iCmdID)
{
	int iRet = 0;

	switch (iCmdID)
	{
	case IDC_CMD_LINE:
		iRet = IDC_CMD_LINE;
		break;
	case IDC_CMD_RECTANGLE:
		iRet = IDC_CMD_RECTANGLE;
		break;
	case IDC_CMD_ELIPPSE:
		iRet = IDC_CMD_ELIPPSE;
		break;
	
	default:
		;
	}

	return iRet;
}
STDMETHODIMP CCommandHandler::UpdateProperty(
	UINT nCmdID,
	REFPROPERTYKEY key,
	const PROPVARIANT* ppropvarCurrentValue,
	PROPVARIANT* ppropvarNewValue)
{

	UNREFERENCED_PARAMETER(ppropvarCurrentValue);
	HRESULT hr = E_NOTIMPL;

	if (key == UI_PKEY_BooleanValue)
	{
		hr = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, g_pApplication->GetCurrentContext() == GetContextMapIDFromCommandID(nCmdID), ppropvarNewValue);

	}

	return hr;
}

//
//  FUNCTION: Execute()
//
//  PURPOSE: Called by the Ribbon framework when a command is executed by the user.  For example, when
//           a button is pressed.
//
STDMETHODIMP CCommandHandler::Execute(
	UINT nCmdID,
	UI_EXECUTIONVERB verb,
	const PROPERTYKEY* key,
	const PROPVARIANT* ppropvarValue,
	IUISimplePropertySet* pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);
	UNREFERENCED_PARAMETER(ppropvarValue);
	UNREFERENCED_PARAMETER(verb);

	HRESULT hr = S_OK;

	if (key != NULL && *key == UI_PKEY_BooleanValue)
	{
		g_pApplication->SetCurrentContext(GetContextMapIDFromCommandID(nCmdID));

		// We need to update the toggle state (boolean value) of the toggle buttons,
		// But we just invalidate all things to keep it simple.
		hr = g_pFramework->InvalidateUICommand(UI_ALL_COMMANDS, UI_INVALIDATIONS_VALUE, NULL);

	}
	switch (nCmdID)
	{
	case IDC_CMD_EXIT:
		DestroyFramework();
		PostQuitMessage(0);
		break;
	case IDC_CMD_NEW:
		Shapes.clear();
		InvalidateRect(g_hwnd, 0, TRUE);
		break;
	case IDC_CMD_SAVE:
		SaveFile();
		break;
	case IDC_CMD_OPEN:
		OpenFile();
		break;
	case IDC_CMD_EXPORT:
		MessageBox(NULL, L"Chức năng xuất ra file ảnh png/bitmap chưa làm được", L"", MB_OK);
	case IDC_CMD_LINE:
		selectedShape = LINE;
		break;
	case IDC_CMD_ELIPPSE:
		selectedShape = ELLIPSE;
		break;
	case IDC_CMD_RECTANGLE:
		selectedShape = RECTANGLE;
		break;
	default:
		break;
	}
	return hr;
}

void SaveFile() {
	OPENFILENAME sfn;
	ZeroMemory(&sfn, sizeof(sfn));
	sfn.lStructSize = sizeof(OPENFILENAME);
	sfn.hwndOwner = NULL;
	sfn.lpstrFile = sysFile;
	sfn.lpstrFile[0] = '\0';
	sfn.nMaxFile = sizeof(sysFile);
	sfn.lpstrFilter = (LPCWSTR)_T("All Files\0*.*\0Text Documents (*.txt)\0*.txt\0");// picture(*.png)\0 * .png\0");
	sfn.lpstrDefExt = _T("txt");
	sfn.nFilterIndex = 2;
	sfn.lpstrFileTitle = NULL;
	sfn.nMaxFileTitle = 0;
	sfn.lpstrInitialDir = NULL;
	sfn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK;
	//InvalidateRect(hwnd, NULL, TRUE);
	if (GetSaveFileName(&sfn)) {

		HANDLE sFile = CreateFile(sfn.lpstrFile, FILE_WRITE_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
		int size = Shapes.size();
		WriteFile(sFile, &size, sizeof(int), &NumOfByteWritten, NULL); //Lưu số lượng phần tử trong vector Shapes
																	   //Lưu từng thông số của mỗi hình vẽ
		for (int i = 0; i < size; i++) {
			//Lưu loại hình vẽ và tọa độ mỗi hình vẽ
			int Type;
			Shapes[i]->GetData(Type, firstX, firstY, lastX, lastY);
			WriteFile(sFile, &Type, sizeof(int), &NumOfByteWritten, NULL);
			WriteFile(sFile, &firstX, sizeof(int), &NumOfByteWritten, NULL);
			WriteFile(sFile, &firstY, sizeof(int), &NumOfByteWritten, NULL);
			WriteFile(sFile, &lastX, sizeof(int), &NumOfByteWritten, NULL);
			WriteFile(sFile, &lastY, sizeof(int), &NumOfByteWritten, NULL);

			//Lưu thông tin bút vẽ
			WriteFile(sFile, &StyleForSelection.color, sizeof(Color), &NumOfByteWritten, NULL);
			WriteFile(sFile, &StyleForSelection.dStyle, sizeof(DashStyle), &NumOfByteWritten, NULL);
			WriteFile(sFile, &StyleForSelection.WidthLine, sizeof(int), &NumOfByteWritten, NULL);

		}
		CloseHandle(sFile);
	}
}

void OpenFile() {
	HDC _hdc = GetDC(g_hwnd);
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = sysFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(sysFile);
	ofn.lpstrFilter = (LPCWSTR)_T("All Files\0*.*\0Text Documents (*.txt)\0*.TXT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK;
	if (GetOpenFileName(&ofn))
	{
		HANDLE oFile = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		int size;
		ReadFile(oFile, &size, sizeof(int), &NumOfByteWritten, NULL);
		Shapes.clear();

		for (int i = 0; i < size; i++)
		{
			int Type;
			ReadFile(oFile, &Type, sizeof(int), &NumOfByteWritten, NULL);
			ReadFile(oFile, &firstX, sizeof(int), &NumOfByteWritten, NULL);
			ReadFile(oFile, &firstY, sizeof(int), &NumOfByteWritten, NULL);
			ReadFile(oFile, &lastX, sizeof(int), &NumOfByteWritten, NULL);
			ReadFile(oFile, &lastY, sizeof(int), &NumOfByteWritten, NULL);

			CStyle tmp;
			ReadFile(oFile, &tmp.color, sizeof(Color), &NumOfByteWritten, NULL);
			ReadFile(oFile, &tmp.dStyle, sizeof(DashStyle), &NumOfByteWritten, NULL);
			ReadFile(oFile, &tmp.WidthLine, sizeof(int), &NumOfByteWritten, NULL);

			switch (Type)
			{
			case LINE:
				curShape = new CLine;
				break;
			case RECTANGLE:
				curShape = new CRectangle;
				break;
			case ELLIPSE:
				curShape = new CElippse;
				break;
			case SQUARE:
				curShape = new CSquare;
				break;
			case CIRCLE:
				curShape = new CCircle;
				break;

			}
			curShape->SetData(firstX, firstY, lastX, lastY);
			curShape->Draw(_hdc, tmp);
			Shapes.push_back(curShape);
		}
		CloseHandle(oFile);
	}
}