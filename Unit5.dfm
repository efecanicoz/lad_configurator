object Form5: TForm5
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Change Password'
  ClientHeight = 112
  ClientWidth = 408
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  Icon.Data = {
    0000010001001818100001000400E80100001600000028000000180000003000
    0000010004000000000020010000000000000000000010000000100000000000
    000000008000008000000080800080000000800080008080000080808000C0C0
    C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FF0
    FF0FF0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000FFFFFF0
    88888888888888880FFFFFF080888888888888880FFFFFF08888888888888888
    0FFFFFF088888888888888880FFFFFF088888888888888880FFFFFF088888888
    888888880FFFFFF088888888888888880FFFFFF088888888888888880FFFFFF0
    00000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0FF0FF0FF0FF0FF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000000000000000000000000000000000000000000000000000}
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object pass1: TMaskEdit
    Left = 24
    Top = 31
    Width = 273
    Height = 21
    PasswordChar = '*'
    TabOrder = 0
    Text = ''
  end
  object StaticText1: TStaticText
    Left = 24
    Top = 14
    Width = 83
    Height = 17
    Caption = 'Enter Password:'
    TabOrder = 1
  end
  object StaticText2: TStaticText
    Left = 24
    Top = 54
    Width = 113
    Height = 17
    Caption = 'Enter Password Again:'
    TabOrder = 2
  end
  object pass2: TMaskEdit
    Left = 24
    Top = 71
    Width = 273
    Height = 21
    PasswordChar = '*'
    TabOrder = 3
    Text = ''
  end
  object ok: TButton
    Left = 312
    Top = 29
    Width = 75
    Height = 25
    Caption = 'Ok'
    TabOrder = 4
    OnClick = okClick
  end
  object Canc: TButton
    Left = 312
    Top = 69
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 5
    OnClick = CancClick
  end
end
