object frmMain: TfrmMain
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 
    'TestSTO - '#1055#1088#1086#1075#1088#1072#1084#1084#1072' '#1084#1086#1076#1077#1083#1080#1088#1091#1102#1097#1072#1103' '#1074#1089#1077#1083#1077#1085#1085#1091#1102' '#1057#1058#1054' '#1080' '#1087#1072#1088#1072#1076#1086#1082#1089' '#1040'.'#1052#1077#1088#1082 +
    #1077#1083#1086#1074#1072' : "'#1055#1086#1089#1083#1077#1076#1085#1080#1081' '#1076#1086#1074#1086#1076' '#1050#1086#1088#1086#1083#1103'"'
  ClientHeight = 726
  ClientWidth = 1016
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object m_PaintBox_AbsSTO: TPaintBox
    Left = 0
    Top = 86
    Width = 1015
    Height = 207
    OnPaint = m_PaintBox_AbsSTOPaint
  end
  object m_PaintBox_MoveSTO: TPaintBox
    Left = 0
    Top = 320
    Width = 1015
    Height = 207
    OnPaint = m_PaintBox_AbsSTOPaint
  end
  object Panel_1: TPanel
    Left = -2
    Top = 60
    Width = 1017
    Height = 27
    Caption = #1053#1077#1087#1086#1076#1074#1080#1078#1085#1072#1103' '#1089#1080#1089#1090#1077#1084#1072' '#1086#1090#1089#1095#1077#1090#1072
    TabOrder = 0
  end
  object Panel_2: TPanel
    Left = -2
    Top = 294
    Width = 1017
    Height = 27
    Caption = #1044#1074#1080#1078#1091#1097#1072#1103#1089#1103' '#1089#1080#1089#1090#1077#1084#1072' '#1086#1090#1089#1095#1077#1090#1072
    TabOrder = 1
  end
  object Panel_3: TPanel
    Left = 0
    Top = 28
    Width = 1015
    Height = 31
    Caption = #1056#1077#1083#1103#1090#1080#1074#1080#1089#1090#1089#1082#1072#1103' '#1092#1080#1079#1080#1082#1072' (C=const '#1086#1090#1085#1086#1089#1080#1090#1077#1083#1100#1085#1086' '#1083#1102#1073#1099#1093' '#1048#1057#1054')'
    Color = clBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 2
  end
  object Panel_4: TPanel
    Left = 0
    Top = 0
    Width = 1017
    Height = 27
    TabOrder = 3
    object m_Label_1: TLabel
      Left = 12
      Top = 7
      Width = 109
      Height = 13
      Caption = #1042#1099#1073#1086#1088' '#1101#1082#1089#1087#1077#1088#1080#1084#1077#1085#1090#1072':'
    end
    object m_ComboBox_Experiment: TComboBox
      Left = 127
      Top = 4
      Width = 410
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      ItemIndex = 4
      TabOrder = 1
      Text = 
        #1056#1077#1096#1072#1102#1097#1080#1081' '#1101#1082#1089#1087#1077#1088#1080#1084#1077#1085#1090' ('#1055#1086#1089#1083#1077#1076#1085#1080#1081' '#1076#1086#1074#1086#1076' '#1050#1086#1088#1086#1083#1103': "'#1063#1072#1089#1099' '#1076#39#1040#1088#1090#1072#1085#1100#1103#1085#1072'"' +
        ')'
      OnChange = m_ComboBox_ExperimentChange
      Items.Strings = (
        #1055#1088#1086#1089#1090#1086#1077' '#1076#1074#1080#1078#1077#1085#1080#1077
        #1054#1090#1085#1086#1089#1080#1090#1077#1083#1100#1085#1086#1089#1090#1100' '#1086#1076#1085#1086#1074#1088#1077#1084#1077#1085#1085#1086#1089#1090#1080
        #1054#1090#1085#1086#1089#1080#1090#1077#1083#1100#1085#1086#1089#1090#1100' '#1086#1076#1085#1086#1074#1088#1077#1084#1077#1085#1085#1086#1089#1090#1080'2'
        #1063#1072#1089#1099' '#1069#1081#1085#1096#1090#1077#1081#1085#1072
        
          #1056#1077#1096#1072#1102#1097#1080#1081' '#1101#1082#1089#1087#1077#1088#1080#1084#1077#1085#1090' ('#1055#1086#1089#1083#1077#1076#1085#1080#1081' '#1076#1086#1074#1086#1076' '#1050#1086#1088#1086#1083#1103': "'#1063#1072#1089#1099' '#1076#39#1040#1088#1090#1072#1085#1100#1103#1085#1072'"' +
          ')')
    end
    object Button_StartExperiment: TButton
      Left = 547
      Top = 5
      Width = 82
      Height = 20
      Hint = 
        #1044#1083#1103' '#1087#1086#1074#1090#1086#1088#1085#1086#1075#1086' '#1079#1072#1087#1091#1089#1082#1072' '#1084#1086#1078#1085#1086' '#1085#1072#1078#1080#1084#1072#1090#1100' '#1082#1085#1086#1087#1082#1091' ENTER '#1085#1072' '#1082#1083#1072#1074#1080#1072#1090#1091#1088#1077 +
        ' ('#1101#1090#1086' '#1091#1076#1086#1073#1085#1077#1077' '#1095#1077#1084' '#1082#1083#1080#1082#1072#1090#1100' '#1084#1099#1096#1082#1086#1081')'
      Caption = #1047#1072#1087#1091#1089#1090#1080#1090#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = Button_StartExperimentClick
    end
    object m_CheckBox_Experiment_FixMoveIso: TCheckBox
      Left = 642
      Top = 6
      Width = 141
      Height = 17
      Caption = #1047#1072#1092#1080#1082#1089#1080#1088#1086#1074#1072#1090#1100' '#1048#1057#1054' K'#39
      TabOrder = 2
      OnClick = m_CheckBox_Experiment_FixMoveIsoClick
    end
    object Button_About: TButton
      Left = 932
      Top = 3
      Width = 85
      Height = 23
      Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
      TabOrder = 3
      OnClick = Button_AboutClick
    end
    object Button_ExpSettings: TButton
      Left = 848
      Top = 3
      Width = 75
      Height = 23
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      TabOrder = 4
      OnClick = Button_ExpSettingsClick
    end
    object Button_ExpHelp: TButton
      Left = 812
      Top = 3
      Width = 33
      Height = 23
      Hint = #1054#1087#1080#1089#1072#1085#1080#1077' '#1090#1077#1082#1091#1097#1077#1075#1086' '#1101#1082#1089#1087#1077#1088#1080#1084#1077#1085#1090#1072
      Caption = #1054
      ParentShowHint = False
      ShowHint = True
      TabOrder = 5
      OnClick = Button_ExpHelpClick
    end
  end
  object Panel1: TPanel
    Left = -2
    Top = 528
    Width = 1017
    Height = 27
    Caption = #1050#1086#1086#1088#1076#1080#1085#1072#1090#1099
    TabOrder = 4
  end
  object m_Memo_Dbg: TMemo
    Left = 0
    Top = 554
    Width = 1015
    Height = 173
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clAqua
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    Lines.Strings = (
      'm_Memo_1')
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 5
  end
end
