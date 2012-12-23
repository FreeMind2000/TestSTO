object frmHello: TfrmHello
  Left = 0
  Top = 0
  BorderIcons = [biMinimize, biMaximize]
  Caption = 
    'TestSTO - '#1055#1088#1086#1075#1088#1072#1084#1084#1072' '#1084#1086#1076#1077#1083#1080#1088#1091#1102#1097#1072#1103' '#1074#1089#1077#1083#1077#1085#1085#1091#1102' '#1057#1058#1054' '#1080' '#1087#1072#1088#1072#1076#1086#1082#1089' '#1040'.'#1052#1077#1088#1082 +
    #1077#1083#1086#1074#1072' : "'#1055#1086#1089#1083#1077#1076#1085#1080#1081' '#1076#1086#1074#1086#1076' '#1050#1086#1088#1086#1083#1103'"'
  ClientHeight = 690
  ClientWidth = 931
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnShow = FormShow
  DesignSize = (
    931
    690)
  PixelsPerInch = 96
  TextHeight = 13
  object m_WebBrowser_1: TWebBrowser
    Left = 4
    Top = 40
    Width = 927
    Height = 597
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
    ExplicitWidth = 903
    ExplicitHeight = 607
    ControlData = {
      4C000000CF5F0000B43D00000000000000000000000000000000000000000000
      000000004C000000000000000000000001000000E0D057007335CF11AE690800
      2B2E126208000000000000004C0000000114020000000000C000000000000046
      8000000000000000000000000000000000000000000000000000000000000000
      00000000000000000100000000000000000000000000000000000000}
  end
  object Button_Next: TButton
    Left = 276
    Top = 654
    Width = 357
    Height = 25
    Anchors = [akLeft, akRight, akBottom]
    Caption = #1055#1077#1088#1077#1081#1090#1080' '#1082' '#1101#1082#1089#1087#1077#1088#1080#1084#1077#1085#1090#1072#1084
    TabOrder = 1
    OnClick = Button_NextClick
  end
  object Button_Back: TButton
    Left = 4
    Top = 12
    Width = 71
    Height = 25
    Caption = '<'
    TabOrder = 2
    OnClick = Button_BackClick
  end
  object Button_Forward: TButton
    Left = 81
    Top = 12
    Width = 71
    Height = 25
    Caption = '>'
    TabOrder = 3
    OnClick = Button_ForwardClick
  end
end
