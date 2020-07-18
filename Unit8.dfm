object Form8: TForm8
  Left = 0
  Top = 0
  Caption = 'Confirm Removal'
  ClientHeight = 97
  ClientWidth = 439
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 24
    Width = 146
    Height = 13
    Caption = 'Choose the variable to delete:'
  end
  object Button1: TButton
    Left = 216
    Top = 35
    Width = 75
    Height = 25
    Caption = 'Delete'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 320
    Top = 35
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Button2Click
  end
  object ComboBox1: TComboBox
    Left = 8
    Top = 51
    Width = 145
    Height = 21
    TabOrder = 2
    Text = 'Variables'
  end
end
