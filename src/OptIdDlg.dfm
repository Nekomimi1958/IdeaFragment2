object OptIndDlg: TOptIndDlg
  Left = 0
  Top = 0
  HelpContext = 22
  BorderStyle = bsDialog
  Caption = #20491#21029#35373#23450
  ClientHeight = 390
  ClientWidth = 594
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  ShowHint = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  DesignSize = (
    594
    390)
  PixelsPerInch = 96
  TextHeight = 13
  object OKBtn: TButton
    Left = 351
    Top = 358
    Width = 89
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 5
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 483
    Top = 358
    Width = 89
    Height = 24
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 6
  end
  object GroupBox1: TGroupBox
    Left = 10
    Top = 104
    Width = 303
    Height = 88
    Caption = #30011#38754#12469#12452#12474'(&S)'
    TabOrder = 1
    object RatioLabel: TLabel
      Left = 131
      Top = 61
      Width = 159
      Height = 18
      AutoSize = False
      Caption = '(1.0000)'
    end
    object ScrWdUpDown: TUpDown
      Left = 88
      Top = 26
      Width = 15
      Height = 21
      Associate = ScrWdEdit
      Min = 100
      Max = 30000
      Position = 800
      TabOrder = 1
      Thousands = False
    end
    object ScrHiUpDown: TUpDown
      Left = 209
      Top = 26
      Width = 15
      Height = 21
      Associate = ScrHiEdit
      Min = 100
      Max = 30000
      Position = 600
      TabOrder = 3
      Thousands = False
    end
    object FixRatioCheckBox: TCheckBox
      Left = 12
      Top = 61
      Width = 113
      Height = 17
      Caption = #32294#27178#27604#12434#22266#23450
      TabOrder = 4
    end
    object ScrWdEdit: TLabeledEdit
      Left = 31
      Top = 26
      Width = 57
      Height = 21
      Alignment = taRightJustify
      EditLabel.Width = 12
      EditLabel.Height = 13
      EditLabel.Caption = #24133
      LabelPosition = lpLeft
      NumbersOnly = True
      TabOrder = 0
      Text = '800'
      OnChange = ScrWdEdit_Change
      OnExit = NumberEditExit
    end
    object ScrHiEdit: TLabeledEdit
      Left = 152
      Top = 26
      Width = 57
      Height = 21
      Alignment = taRightJustify
      EditLabel.Width = 20
      EditLabel.Height = 13
      EditLabel.Caption = #39640#12373
      LabelPosition = lpLeft
      NumbersOnly = True
      TabOrder = 2
      Text = '600'
      OnChange = ScrHiEdit_Change
      OnExit = NumberEditExit
    end
  end
  object GroupBox2: TGroupBox
    Left = 332
    Top = 104
    Width = 250
    Height = 138
    Caption = #30011#38754#20998#21106#32218'(&D)'
    TabOrder = 3
    DesignSize = (
      250
      138)
    object TLabel
      Left = 26
      Top = 28
      Width = 26
      Height = 13
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = #12479#12452#12503
    end
    object TLabel
      Left = 42
      Top = 108
      Width = 24
      Height = 13
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = #32218#24133
    end
    object DivPrm1UpDown: TUpDown
      Left = 118
      Top = 64
      Width = 15
      Height = 21
      Associate = DivPrm1Edit
      TabOrder = 2
      Thousands = False
    end
    object DivPrm2UpDown: TUpDown
      Left = 220
      Top = 64
      Width = 15
      Height = 21
      Associate = DivPrm2Edit
      TabOrder = 4
      Thousands = False
    end
    object DivModComboBox: TComboBox
      Left = 56
      Top = 24
      Width = 183
      Height = 21
      Style = csDropDownList
      TabOrder = 0
      OnChange = DivModComboBoxChange
    end
    object DivLwComboBox: TComboBox
      Left = 72
      Top = 104
      Width = 63
      Height = 21
      Style = csDropDownList
      TabOrder = 5
    end
    object DivPrm1Edit: TLabeledEdit
      Left = 72
      Top = 64
      Width = 46
      Height = 21
      Alignment = taRightJustify
      EditLabel.Width = 12
      EditLabel.Height = 13
      EditLabel.Caption = #27178
      LabelPosition = lpLeft
      NumbersOnly = True
      TabOrder = 1
      Text = '0'
      OnChange = DivEditChange
    end
    object DivPrm2Edit: TLabeledEdit
      Left = 174
      Top = 64
      Width = 46
      Height = 21
      Alignment = taRightJustify
      EditLabel.Width = 12
      EditLabel.Height = 13
      EditLabel.Caption = #32294
      LabelPosition = lpLeft
      NumbersOnly = True
      TabOrder = 3
      Text = '0'
      OnChange = DivEditChange
    end
  end
  object GroupBox3: TGroupBox
    Left = 10
    Top = 197
    Width = 303
    Height = 88
    Caption = #37197#33394'(&C)'
    TabOrder = 2
    DesignSize = (
      303
      88)
    object ColComboBox: TComboBox
      Left = 12
      Top = 22
      Width = 280
      Height = 22
      Style = csOwnerDrawFixed
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnChange = ColComboBoxChange
      OnDrawItem = ColComboBoxDrawItem
    end
    object ColButton: TButton
      Left = 165
      Top = 54
      Width = 40
      Height = 24
      Anchors = [akTop, akRight]
      Caption = '...'
      Enabled = False
      TabOrder = 1
      OnClick = ColButtonClick
    end
    object DefColBtn: TButton
      Left = 210
      Top = 54
      Width = 80
      Height = 24
      Anchors = [akTop, akRight]
      Caption = #12487#12501#12457#12523#12488
      TabOrder = 2
      OnClick = DefColBtnClick
    end
  end
  object GroupBox4: TGroupBox
    Left = 10
    Top = 291
    Width = 303
    Height = 88
    Caption = #12501#12457#12531#12488'(&T)'
    TabOrder = 4
    DesignSize = (
      303
      88)
    object FontButton: TButton
      Left = 165
      Top = 54
      Width = 40
      Height = 24
      Anchors = [akTop, akRight]
      Caption = '...'
      Enabled = False
      TabOrder = 1
      OnClick = FontButtonClick
    end
    object FontComboBox: TComboBox
      Left = 12
      Top = 22
      Width = 280
      Height = 22
      Style = csOwnerDrawFixed
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnChange = FontComboBoxChange
      OnDrawItem = FontComboBoxDrawItem
    end
    object DefFontBtn: TButton
      Left = 210
      Top = 54
      Width = 80
      Height = 24
      Anchors = [akTop, akRight]
      Caption = #12487#12501#12457#12523#12488
      TabOrder = 2
      OnClick = DefFontBtnClick
    end
  end
  object GroupBox5: TGroupBox
    Left = 10
    Top = 8
    Width = 572
    Height = 83
    Caption = #12479#12452#12488#12523'(&T)'
    TabOrder = 0
    DesignSize = (
      572
      83)
    object TLabel
      Left = 38
      Top = 54
      Width = 48
      Height = 13
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = #34920#31034#20301#32622
    end
    object TitleEdit: TEdit
      Left = 15
      Top = 20
      Width = 543
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
    object TitPosComboBox: TComboBox
      Left = 89
      Top = 50
      Width = 137
      Height = 21
      Style = csDropDownList
      TabOrder = 1
      Items.Strings = (
        #38750#34920#31034
        #24038#19978#38533
        #19978#20013#22830
        #21491#19978#38533
        #24038#20013#22830
        #20013#22830
        #21491#20013#22830
        #24038#19979#38533
        #19979#20013#22830
        #21491#19979#38533)
    end
  end
  object ColorDialog1: TColorDialog
    Left = 440
    Top = 268
  end
  object FontDialog1: TFontDialog
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Style = []
    Left = 360
    Top = 268
  end
end
