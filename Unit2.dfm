object Frame2: TFrame2
  Left = 0
  Top = 0
  Width = 500
  Height = 47
  TabOrder = 0
  object Variable: TLabel
    Left = 22
    Top = 17
    Width = 27
    Height = 19
    Caption = 'X00'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 55
    Top = 20
    Width = 40
    Height = 13
    Caption = '= true if'
  end
  object Inp: TLabel
    Left = 108
    Top = 3
    Width = 55
    Height = 13
    Caption = 'Input Value'
  end
  object Label3: TLabel
    Left = 174
    Top = 3
    Width = 45
    Height = 13
    Caption = 'Condition'
  end
  object Label4: TLabel
    Left = 239
    Top = 3
    Width = 26
    Height = 13
    Caption = 'Value'
  end
  object Input: TComboBox
    Left = 108
    Top = 17
    Width = 61
    Height = 21
    ItemIndex = 0
    TabOrder = 0
    Text = 'I01'
    OnKeyPress = boxKeyPress
    OnSelect = InputSelect
    Items.Strings = (
      'I01'
      'I02'
      'I03'
      'I04'
      'I05')
  end
  object Condition: TComboBox
    Left = 173
    Top = 17
    Width = 61
    Height = 21
    TabOrder = 1
    Text = '<'
    OnKeyPress = boxKeyPress
    OnSelect = ConditionSelect
    Items.Strings = (
      '<'
      '>'
      '=')
  end
  object rg: TRadioGroup
    Left = 325
    Top = 3
    Width = 156
    Height = 38
    Caption = 'The value is set as'
    Columns = 2
    ItemIndex = 0
    Items.Strings = (
      '% of VCC'
      'Volts')
    TabOrder = 2
    OnClick = rgClick
  end
  object value: TCSpinEdit
    Left = 238
    Top = 17
    Width = 81
    Height = 22
    MaxValue = 100
    TabOrder = 3
    OnChange = valueChange
  end
end
