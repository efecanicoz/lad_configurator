object Form7: TForm7
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Change LAD revision'
  ClientHeight = 65
  ClientWidth = 307
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
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
  object rev: TCSpinEdit
    Left = 16
    Top = 24
    Width = 118
    Height = 22
    MaxValue = 99
    MinValue = 1
    TabOrder = 0
    Value = 1
    OnKeyPress = revKeyPress
  end
  object Button1: TButton
    Left = 140
    Top = 22
    Width = 75
    Height = 25
    Caption = 'Ok'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 221
    Top = 22
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = Button2Click
  end
end
