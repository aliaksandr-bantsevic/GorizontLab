object Form_Port_adjust: TForm_Port_adjust
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1087#1086#1088#1090
  ClientHeight = 267
  ClientWidth = 395
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object Label1: TLabel
    Left = 230
    Top = 11
    Width = 50
    Height = 15
    Caption = 'BaudRate'
  end
  object Button_ok: TButton
    Left = 300
    Top = 234
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = Button_okClick
  end
  object Button_cancel: TButton
    Left = 205
    Top = 234
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Button_cancelClick
  end
  object ComboBox_COM: TComboBox
    Left = 103
    Top = 8
    Width = 90
    Height = 23
    TabOrder = 2
    Text = 'ComboBox_COM'
  end
  object ComboBox_type: TComboBox
    Left = 8
    Top = 8
    Width = 89
    Height = 23
    TabOrder = 3
    Text = 'ComboBox_type'
  end
  object Memo_mark: TMemo
    Left = 11
    Top = 139
    Width = 373
    Height = 78
    Lines.Strings = (
      'Memo_mark')
    TabOrder = 4
  end
  object ComboBox_baud: TComboBox
    Left = 300
    Top = 8
    Width = 87
    Height = 23
    TabOrder = 5
    Text = 'ComboBox_baud'
  end
  object CheckBox_on: TCheckBox
    Left = 8
    Top = 242
    Width = 49
    Height = 17
    Caption = #1042#1082#1083'.'
    Checked = True
    State = cbChecked
    TabOrder = 6
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 37
    Width = 185
    Height = 96
    Caption = #1058#1072#1081#1084'-'#1072#1091#1090', '#1084#1089
    TabOrder = 7
    object Label2: TLabel
      Left = 15
      Top = 32
      Width = 39
      Height = 15
      Caption = #1063#1090#1077#1085#1080#1077
    end
    object Label3: TLabel
      Left = 15
      Top = 61
      Width = 39
      Height = 15
      Caption = #1047#1072#1087#1080#1089#1100
    end
    object ComboBox_tout_rd: TComboBox
      Left = 88
      Top = 24
      Width = 81
      Height = 23
      TabOrder = 0
      Text = 'ComboBox_tout_rd'
    end
    object ComboBox_tout_wr: TComboBox
      Left = 88
      Top = 53
      Width = 81
      Height = 23
      TabOrder = 1
      Text = 'ComboBox_tout_wr'
    end
  end
  object GroupBox2: TGroupBox
    Left = 199
    Top = 37
    Width = 185
    Height = 96
    Caption = #1047#1072#1076#1077#1088#1078#1082#1072', '#1084#1089
    TabOrder = 8
    object Label4: TLabel
      Left = 16
      Top = 32
      Width = 38
      Height = 15
      Caption = 'Default'
    end
    object Label5: TLabel
      Left = 19
      Top = 61
      Width = 59
      Height = 15
      Caption = 'Cmd. exec.'
    end
    object ComboBox_dl_df: TComboBox
      Left = 96
      Top = 24
      Width = 81
      Height = 23
      TabOrder = 0
      Text = 'ComboBox_dl_df'
    end
    object ComboBox_dl_ex: TComboBox
      Left = 96
      Top = 53
      Width = 81
      Height = 23
      TabOrder = 1
      Text = 'ComboBox_dl_ex'
    end
  end
  object Timer_start: TTimer
    Interval = 100
    OnTimer = Timer_startTimer
    Left = 160
    Top = 168
  end
end
