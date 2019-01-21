object RubLayDlg: TRubLayDlg
  Left = 0
  Top = 0
  HelpContext = 37
  BorderStyle = bsToolWindow
  Caption = #28014#36938#37197#32622
  ClientHeight = 189
  ClientWidth = 232
  Color = clBtnFace
  ParentFont = True
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object RunBtn: TSpeedButton
    Left = 121
    Top = 152
    Width = 52
    Height = 26
    Action = RunAction
    GroupIndex = 1
  end
  object RdyBtn: TSpeedButton
    Left = 174
    Top = 152
    Width = 52
    Height = 26
    Action = ReadyAction
    GroupIndex = 1
    Down = True
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 77
    Width = 217
    Height = 60
    Caption = #26029#29255#38291#36317#38626
    TabOrder = 1
    object Label2: TLabel
      Left = 12
      Top = 24
      Width = 12
      Height = 13
      Caption = #30701
    end
    object Label3: TLabel
      Left = 190
      Top = 24
      Width = 12
      Height = 13
      Caption = #38263
    end
    object SpcTrackBar: TTrackBar
      Left = 28
      Top = 20
      Width = 160
      Height = 33
      Max = 1000
      Min = 20
      Frequency = 98
      Position = 20
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 4
    Width = 217
    Height = 61
    Caption = #38306#20418#32218#38263
    TabOrder = 0
    object Label1: TLabel
      Left = 12
      Top = 24
      Width = 12
      Height = 13
      Caption = #30701
    end
    object Label4: TLabel
      Left = 190
      Top = 24
      Width = 12
      Height = 13
      Caption = #38263
    end
    object LenTrackBar: TTrackBar
      Left = 29
      Top = 20
      Width = 160
      Height = 29
      Max = 500
      Min = 10
      Frequency = 49
      Position = 10
      TabOrder = 0
    end
  end
  object ToolBar1: TToolBar
    Left = 7
    Top = 152
    Width = 108
    Height = 26
    Align = alNone
    AutoSize = True
    ButtonHeight = 26
    ButtonWidth = 27
    Caption = 'ToolBar1'
    Images = IdeaFragMainForm.BtnImgList
    TabOrder = 2
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Action = InvVAction
    end
    object ToolButton2: TToolButton
      Left = 27
      Top = 0
      Action = InvHAcion
    end
    object ToolButton3: TToolButton
      Left = 54
      Top = 0
      Action = RotLAction
    end
    object ToolButton4: TToolButton
      Left = 81
      Top = 0
      Action = RotRAction
    end
  end
  object HideBtn: TButton
    Left = 0
    Top = 189
    Width = 232
    Height = 0
    Align = alBottom
    Cancel = True
    TabOrder = 3
    OnClick = HideBtnClick
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 50
    OnTimer = Timer1Timer
    Left = 176
    Top = 56
  end
  object ActionList1: TActionList
    Images = IdeaFragMainForm.BtnImgList
    Left = 136
    Top = 56
    object RunAction: TAction
      Caption = #26377#21177
      GroupIndex = 1
      HelpType = htContext
      OnExecute = RunActionExecute
      OnUpdate = RunActionUpdate
    end
    object ReadyAction: TAction
      Caption = #24453#27231
      GroupIndex = 1
      HelpType = htContext
      OnExecute = ReadyActionExecute
    end
    object InvVAction: TAction
      Tag = 1
      HelpType = htContext
      Hint = #19978#19979#21453#36578
      ImageIndex = 29
      OnExecute = QuickActionExecute
      OnUpdate = QuickActionUpdate
    end
    object InvHAcion: TAction
      Tag = 2
      HelpType = htContext
      Hint = #24038#21491#21453#36578
      ImageIndex = 30
      OnExecute = QuickActionExecute
      OnUpdate = QuickActionUpdate
    end
    object RotLAction: TAction
      Tag = -90
      HelpType = htContext
      Hint = #24038#12408'90'#24230#22238#36578
      ImageIndex = 31
      OnExecute = QuickActionExecute
      OnUpdate = QuickActionUpdate
    end
    object RotRAction: TAction
      Tag = 90
      HelpType = htContext
      Hint = #21491#12408'90'#24230#22238#36578
      ImageIndex = 32
      OnExecute = QuickActionExecute
      OnUpdate = QuickActionUpdate
    end
  end
end
