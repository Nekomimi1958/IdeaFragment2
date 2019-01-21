object StretchDlg: TStretchDlg
  Left = 0
  Top = 0
  HelpContext = 6
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  BorderWidth = 6
  ClientHeight = 199
  ClientWidth = 258
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel4: TPanel
    Left = 0
    Top = 165
    Width = 258
    Height = 34
    Align = alBottom
    AutoSize = True
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 3
    DesignSize = (
      258
      34)
    object CancelBtn: TButton
      Left = 85
      Top = 4
      Width = 89
      Height = 26
      Anchors = [akLeft, akBottom]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 1
    end
    object OKBtn: TButton
      Left = 12
      Top = 4
      Width = 65
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object ApplyBtn: TButton
      Left = 182
      Top = 4
      Width = 65
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = #36969#29992
      TabOrder = 2
      OnClick = ApplyBtnClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 258
    Height = 53
    Align = alTop
    AutoSize = True
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 0
    object Label1: TLabel
      Left = 4
      Top = 8
      Width = 4
      Height = 13
      Caption = '-'
    end
    object Label2: TLabel
      Left = 240
      Top = 8
      Width = 8
      Height = 13
      Caption = '+'
    end
    object SetBtn1: TButton
      Left = 18
      Top = 32
      Width = 17
      Height = 17
      Caption = '2'
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = SetBtnXClick
    end
    object TrackBar1: TTrackBar
      Left = 14
      Top = 4
      Width = 224
      Height = 27
      Max = 200
      Min = -200
      PageSize = 25
      Frequency = 100
      TabOrder = 0
      OnChange = TrackBar1Change
    end
    object SetBtn2: TButton
      Tag = 1
      Left = 68
      Top = 32
      Width = 17
      Height = 17
      Caption = '1'
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = SetBtnXClick
    end
    object SetBtn3: TButton
      Tag = 2
      Left = 118
      Top = 32
      Width = 17
      Height = 17
      Caption = '0'
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = SetBtnXClick
    end
    object SetBtn4: TButton
      Tag = 3
      Left = 168
      Top = 32
      Width = 17
      Height = 17
      Caption = '1'
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = SetBtnXClick
    end
    object SetBtn5: TButton
      Tag = 4
      Left = 218
      Top = 32
      Width = 17
      Height = 17
      Caption = '2'
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      OnClick = SetBtnXClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 53
    Width = 258
    Height = 80
    AutoSize = True
    BevelOuter = bvNone
    BorderWidth = 4
    TabOrder = 1
    object RadioGroup1: TRadioGroup
      Left = 4
      Top = 4
      Width = 250
      Height = 72
      Align = alClient
      Caption = #26041#21521
      Columns = 3
      Items.Strings = (
        #25918#23556'(&C)'
        #26012#12417'(&L)'
        #24038#21491'(&H)'
        #19978#19979'(&V)'
        #22238#36578'(&R)'
        #32294#27178'(&X)')
      TabOrder = 0
      OnClick = RadioGroup1Click
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 133
    Width = 258
    Height = 32
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    object Grp0CheckBox: TCheckBox
      Left = 21
      Top = 7
      Width = 217
      Height = 17
      Caption = #28961#25152#23646#26029#29255#12418#36899#21205#12375#12390#20280#32302
      TabOrder = 0
    end
  end
end
