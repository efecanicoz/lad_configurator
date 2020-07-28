object Form9: TForm9
  Left = 0
  Top = 0
  Caption = 'Counter '
  ClientHeight = 107
  ClientWidth = 316
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object label_counter: TLabel
    Left = 24
    Top = 19
    Width = 43
    Height = 13
    Caption = 'Counter:'
  end
  object label_reset: TLabel
    Left = 24
    Top = 48
    Width = 61
    Height = 13
    Caption = 'Reset value:'
  end
  object counterDDL: TComboBox
    Left = 88
    Top = 16
    Width = 145
    Height = 21
    TabOrder = 0
    Text = 'Counters'
  end
  object input_reset: TCSpinEdit
    Left = 112
    Top = 43
    Width = 121
    Height = 22
    MaxValue = 65535
    TabOrder = 1
  end
  object Button1: TButton
    Left = 112
    Top = 71
    Width = 75
    Height = 25
    Caption = 'Add'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 200
    Top = 71
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 3
    OnClick = Button2Click
  end
end
