#include "Input.h"

#include <cassert>

using namespace Microsoft::WRL;

void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{

    HRESULT result;

    // DirectInput�̏�����
    ComPtr<IDirectInput8> directInput;
    result = DirectInput8Create(
        hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

    // �L�[�{�[�h�f�o�C�X�̐���
   
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    // ���̓f�[�^�`���̃Z�b�g
    result = keyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
    assert(SUCCEEDED(result));
    // �r�����䃌�x���̃Z�b�g
    result = keyboard->SetCooperativeLevel(
        hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(result));


}


void Input::Update()
{

    //keyPre�̒���key�̏����R�s�[
    memcpy(keyPre, key, sizeof(key));
    // �L�[�{�[�h���̎擾�J�n
    keyboard->Acquire();
    // �S�L�[�̓��͏�Ԃ��擾����
   
    keyboard->GetDeviceState(sizeof(key), key);

}


bool Input::PushKeys(BYTE keyNumber)
{
    
    //�C�ӂ̃{�^����������Ă��邩
    if (key[keyNumber])
    {

        return true;

    }

    //�C�ӂ̃{�^����������Ă��Ȃ������Ƃ�
    return false;

}

bool Input::TriggerKey(BYTE keyNumber)
{
    if (keyPre[keyNumber] && keyPre[keyNumber] == 0)
    {

        return true;
    }

    return false;
}
