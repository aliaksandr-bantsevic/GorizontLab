object Form_Place_adjust: TForm_Place_adjust
  Left = 0
  Top = 0
  Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1084#1077#1089#1090#1086
  ClientHeight = 166
  ClientWidth = 620
  Color = clBtnFace
  Font.Charset = RUSSIAN_CHARSET
  Font.Color = clWindowText
  Font.Height = -15
  Font.Name = '@Arial Unicode MS'
  Font.Style = []
  TextHeight = 20
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 78
    Height = 20
    Caption = #1048#1084#1103' '#1084#1077#1089#1090#1072':'
  end
  object Label2: TLabel
    Left = 8
    Top = 40
    Width = 127
    Height = 20
    Caption = #1050#1088#1072#1090#1082#1086#1077' '#1086#1087#1080#1089#1072#1085#1080#1077':'
  end
  object Button_ok: TButton
    Left = 537
    Top = 132
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = Button_okClick
  end
  object Button_cancel: TButton
    Left = 445
    Top = 132
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
  end
  object Edit_name: TEdit
    Left = 160
    Top = 8
    Width = 449
    Height = 28
    TabOrder = 2
  end
  object Memo_mark: TMemo
    Left = 160
    Top = 37
    Width = 449
    Height = 89
    Lines.Strings = (
      'Memo_mark')
    ScrollBars = ssVertical
    TabOrder = 3
  end
  object Timer_start: TTimer
    Enabled = False
    Interval = 100
    OnTimer = Timer_startTimer
    Left = 32
    Top = 80
  end
end
