﻿object Form_Sensor_adjust: TForm_Sensor_adjust
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1076#1072#1090#1095#1080#1082
  ClientHeight = 345
  ClientWidth = 477
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object Label1: TLabel
    Left = 127
    Top = 8
    Width = 7
    Height = 15
    Caption = '#'
  end
  object Протокол: TLabel
    Left = 58
    Top = 42
    Width = 55
    Height = 15
    Caption = #1055#1088#1086#1090#1086#1082#1086#1083
  end
  object Label5: TLabel
    Left = 160
    Top = 322
    Width = 136
    Height = 15
    Caption = 'data streeam rate, times/s'
  end
  object Button_OK: TButton
    Left = 394
    Top = 315
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = Button_OKClick
  end
  object Button_cancel: TButton
    Left = 313
    Top = 315
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Button_cancelClick
  end
  object ComboBox_addr: TComboBox
    Left = 142
    Top = 5
    Width = 108
    Height = 23
    TabOrder = 2
    Text = 'ComboBox_addr'
    OnChange = ComboBox_addrChange
  end
  object ComboBox_type: TComboBox
    Left = 8
    Top = 5
    Width = 105
    Height = 23
    TabOrder = 3
    Text = 'ComboBox_type'
  end
  object Memo_mark: TMemo
    Left = 8
    Top = 63
    Width = 461
    Height = 55
    Lines.Strings = (
      'Memo_mark')
    TabOrder = 4
  end
  object CheckBox_on: TCheckBox
    Left = 23
    Top = 315
    Width = 49
    Height = 17
    Caption = #1042#1082#1083'.'
    Checked = True
    State = cbChecked
    TabOrder = 5
  end
  object ComboBox_prt: TComboBox
    Left = 142
    Top = 34
    Width = 109
    Height = 23
    TabOrder = 6
    Text = 'ComboBox_prt'
  end
  object ComboBox_addr1: TComboBox
    Left = 256
    Top = 5
    Width = 113
    Height = 23
    TabOrder = 7
    Text = 'ComboBox_addr1'
  end
  object Edit_list: TEdit
    Left = 140
    Top = 5
    Width = 229
    Height = 23
    TabOrder = 8
    Visible = False
  end
  object CheckBox_list: TCheckBox
    Left = 391
    Top = 8
    Width = 78
    Height = 17
    Caption = #1057#1087#1080#1089#1086#1082
    TabOrder = 9
    OnClick = CheckBox_listClick
  end
  object PageControl_ext: TPageControl
    Left = 8
    Top = 124
    Width = 461
    Height = 185
    ActivePage = TabSheet_ind3
    TabOrder = 10
    object TabSheet_ind3: TTabSheet
      Caption = #1048#1053'-'#1044'3'
      object GroupBox1: TGroupBox
        Left = 3
        Top = 0
        Width = 214
        Height = 125
        Caption = #1057#1084#1077#1097#1077#1085#1080#1077' '#1085#1091#1083#1103' '#1087#1086#1089#1083#1077' '#1102#1089#1090#1080#1088#1086#1074#1082#1080
        TabOrder = 0
        object Label2: TLabel
          Left = 8
          Top = 40
          Width = 7
          Height = 15
          Caption = 'X'
        end
        object Label3: TLabel
          Left = 8
          Top = 88
          Width = 7
          Height = 15
          Caption = 'Y'
        end
        object Edit_X: TEdit
          Left = 72
          Top = 32
          Width = 121
          Height = 23
          TabOrder = 0
        end
        object Edit_Y: TEdit
          Left = 72
          Top = 80
          Width = 121
          Height = 23
          TabOrder = 1
        end
      end
      object GroupBox2: TGroupBox
        Left = 223
        Top = 0
        Width = 227
        Height = 125
        Caption = #1055#1088#1080#1074#1077#1076#1077#1085#1080#1077' '#1082#1086#1086#1088#1076#1080#1085#1072#1090
        TabOrder = 1
        object Label4: TLabel
          Left = 16
          Top = 107
          Width = 197
          Height = 15
          Caption = #1059#1075#1086#1083' '#1074#1088#1072#1097#1077#1085#1080#1103' '#1089#1080#1089#1090#1077#1084#1099' '#1082#1086#1086#1088#1076#1080#1085#1072#1090
        end
        object CheckBox1: TCheckBox
          Left = 19
          Top = 32
          Width = 185
          Height = 17
          Caption = #1048#1085#1074#1077#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1086#1089#1100' Y'
          TabOrder = 0
        end
        object Edit_round: TEdit
          Left = 19
          Top = 78
          Width = 121
          Height = 23
          TabOrder = 1
        end
      end
      object CheckBox2: TCheckBox
        Left = 11
        Top = 135
        Width = 334
        Height = 17
        Caption = #1055#1088#1080#1084#1077#1085#1103#1090#1100' '#1076#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1099#1077' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
        TabOrder = 2
      end
    end
    object TabSheet_and3: TTabSheet
      Caption = #1040#1053'-'#1044'3'
      ImageIndex = 1
    end
  end
  object ComboBox_rate: TComboBox
    Left = 78
    Top = 314
    Width = 66
    Height = 23
    TabOrder = 11
  end
  object Timer_start: TTimer
    Interval = 100
    OnTimer = Timer_startTimer
    Left = 192
    Top = 72
  end
end
