object Form8: TForm8
  Left = 0
  Top = 0
  Caption = 'Confirm Removal'
  ClientHeight = 72
  ClientWidth = 439
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = BeforeActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 24
    Width = 165
    Height = 13
    Caption = 'You are about to delete variable : '
  end
  object Button1: TButton
    Left = 232
    Top = 19
    Width = 75
    Height = 25
    Caption = 'Delete'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 328
    Top = 19
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Button2Click
  end
end
