object FrgPropDlg: TFrgPropDlg
  Left = 0
  Top = 0
  HelpContext = 32
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Anchors = []
  Caption = #12503#12525#12497#12486#12451
  ClientHeight = 336
  ClientWidth = 376
  Color = clBtnFace
  Constraints.MinHeight = 340
  Constraints.MinWidth = 384
  ParentFont = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 165
    Width = 376
    Height = 171
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      376
      171)
    object GripPaintBox: TPaintBox
      Left = 340
      Top = 135
      Width = 36
      Height = 36
      Anchors = [akRight, akBottom]
      OnPaint = GripPaintBoxPaint
    end
    object OkBtn: TButton
      Left = 86
      Top = 137
      Width = 85
      Height = 26
      Action = OkAction
      Anchors = [akLeft, akBottom]
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object CancelBtn: TButton
      Left = 204
      Top = 137
      Width = 87
      Height = 26
      Anchors = [akLeft, akBottom]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 1
    end
    object ColGroupBox: TGroupBox
      Left = 236
      Top = 10
      Width = 133
      Height = 53
      Anchors = [akLeft, akBottom]
      Caption = #37197#33394'(&C)'
      TabOrder = 3
      DesignSize = (
        133
        53)
      object BackColorBtn: TButton
        Left = 70
        Top = 21
        Width = 55
        Height = 24
        Action = BackColAction
        TabOrder = 1
      end
      object CharColorBtn: TButton
        Left = 8
        Top = 21
        Width = 55
        Height = 24
        Action = CharColAction
        TabOrder = 0
      end
      object TrnsChkBox: TCheckBox
        Left = 16
        Top = 25
        Width = 48
        Height = 17
        Anchors = [akLeft, akBottom]
        Caption = #36879#36942
        TabOrder = 2
        OnClick = TrnsChkBoxClick
      end
    end
    object SizeGroupBox: TGroupBox
      Left = 8
      Top = 10
      Width = 221
      Height = 53
      Anchors = [akLeft, akBottom]
      Caption = #12469#12452#12474'(&S)'
      TabOrder = 2
      object SizePer_Label: TLabel
        Left = 98
        Top = 25
        Width = 12
        Height = 13
        Caption = #65295
      end
      object WdUpDown: TUpDown
        Left = 62
        Top = 21
        Width = 15
        Height = 21
        Associate = FrgWdEdit
        Min = 1
        Max = 40
        Position = 12
        TabOrder = 1
      end
      object HiUpDown: TUpDown
        Left = 167
        Top = 21
        Width = 15
        Height = 21
        Associate = FrgHiEdit
        Min = 1
        Max = 50
        Position = 3
        TabOrder = 4
      end
      object FitHiBtn: TButton
        Left = 183
        Top = 21
        Width = 32
        Height = 22
        Action = FitHiAction
        TabOrder = 5
      end
      object FitWdBtn: TButton
        Left = 78
        Top = 21
        Width = 32
        Height = 22
        Action = FitWdAction
        TabOrder = 2
      end
      object FrgWdEdit: TLabeledEdit
        Left = 29
        Top = 21
        Width = 33
        Height = 21
        Alignment = taRightJustify
        EditLabel.Width = 12
        EditLabel.Height = 13
        EditLabel.Caption = #26689
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 0
        Text = '12'
        OnChange = FrgWdEdit_Change
      end
      object FrgHiEdit: TLabeledEdit
        Left = 134
        Top = 21
        Width = 33
        Height = 21
        Alignment = taRightJustify
        EditLabel.Width = 12
        EditLabel.Height = 13
        EditLabel.Caption = #34892
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 3
        Text = '3'
        OnChange = FrgHiEditChange
      end
    end
    object LinkGrpBox: TGroupBox
      Left = 8
      Top = 72
      Width = 361
      Height = 53
      Anchors = [akLeft, akRight, akBottom]
      Caption = #12522#12531#12463'(&L)'
      TabOrder = 4
      DesignSize = (
        361
        53)
      object LinkNameEdit: TEdit
        Left = 8
        Top = 22
        Width = 282
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        ImeMode = imClose
        TabOrder = 0
      end
      object LinkRefBtn: TButton
        Left = 291
        Top = 21
        Width = 36
        Height = 22
        Action = RefLinkAction
        Anchors = [akTop, akRight]
        TabOrder = 1
      end
      object LnkExBtn: TButton
        Left = 329
        Top = 21
        Width = 24
        Height = 22
        Action = GetLnkInfAction
        Anchors = [akTop, akRight]
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 376
    Height = 165
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      376
      165)
    object GroupLabel: TLabel
      Left = 36
      Top = 10
      Width = 53
      Height = 13
      Alignment = taRightJustify
      Caption = #12464#12523#12540#12503'(&G)'
      FocusControl = GroupComboBox
    end
    object Label1: TLabel
      Left = 8
      Top = 35
      Width = 38
      Height = 13
      Caption = #20869#23481'(&E)'
      FocusControl = Memo1
    end
    object Memo1: TMemo
      Left = 8
      Top = 54
      Width = 274
      Height = 111
      Anchors = [akLeft, akTop, akRight, akBottom]
      MaxLength = 1024
      ScrollBars = ssVertical
      TabOrder = 1
      WantReturns = False
      WordWrap = False
      OnChange = Memo1Change
      OnEnter = Memo1Enter
      OnExit = Memo1Exit
    end
    object TypeGroupBox: TGroupBox
      Left = 297
      Top = 42
      Width = 72
      Height = 123
      Anchors = [akTop, akRight]
      Caption = #12479#12452#12503'(&T)'
      TabOrder = 2
      object NormalRadioBtn: TRadioButton
        Left = 8
        Top = 24
        Width = 57
        Height = 17
        Action = TypeFrgAction
        TabOrder = 0
      end
      object PlateRadioBtn: TRadioButton
        Left = 8
        Top = 48
        Width = 57
        Height = 17
        Action = TypePltAction
        TabOrder = 1
      end
      object SymbolRadioBtn: TRadioButton
        Left = 8
        Top = 72
        Width = 57
        Height = 17
        Action = TypeSymAction
        TabOrder = 2
      end
      object ImageRadioBtn: TRadioButton
        Left = 8
        Top = 96
        Width = 57
        Height = 17
        Action = TypeImgAction
        TabOrder = 3
      end
    end
    object GroupComboBox: TComboBox
      Left = 96
      Top = 6
      Width = 273
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnChange = GroupComboBoxChange
    end
  end
  object ColorDialog1: TColorDialog
    Left = 160
    Top = 64
  end
  object OpenLnkDlg: TOpenDialog
    Left = 40
    Top = 64
  end
  object PopupMenu1: TPopupMenu
    OnPopup = PopupMenu1Popup
    Left = 40
    Top = 112
    object PopGetTitItem: TMenuItem
      Caption = #12479#12452#12488#12523' (&T)'
      OnClick = PopGetTitItemClick
    end
    object PopGetDscItem: TMenuItem
      Caption = 'META: description (&D)'
      OnClick = PopGetMetaItemClick
    end
    object PopGetKeyItem: TMenuItem
      Tag = 1
      Caption = 'META: keywords (&K)'
      OnClick = PopGetMetaItemClick
    end
    object PopGetTopItem: TMenuItem
      Caption = #26377#29992#12381#12358#12394#26368#21021#12398#34892' (&F)'
      OnClick = PopGetTopItemClick
    end
    object PopGetTrakItem: TMenuItem
      Caption = #12488#12521#12483#12463#21517' (&T)'
      OnClick = PopGetMp3ItemClick
    end
    object PopGetArtistItem: TMenuItem
      Tag = 1
      Caption = #12450#12540#12486#12451#12473#12488#21517' (&A)'
      OnClick = PopGetMp3ItemClick
    end
    object PopGetAlbumItem: TMenuItem
      Tag = 2
      Caption = #12450#12523#12496#12512#21517' (&L)'
      OnClick = PopGetMp3ItemClick
    end
    object PopGetYearItem: TMenuItem
      Tag = 3
      Caption = #12522#12522#12540#12473#24180#21495' (&Y)'
      OnClick = PopGetMp3ItemClick
    end
    object PopGetComItem: TMenuItem
      Tag = 4
      Caption = #12467#12513#12531#12488' (&C)'
      OnClick = PopGetMp3ItemClick
    end
    object PopGetPageItem: TMenuItem
      Caption = #12506#12540#12472#21517'(&G)'
      OnClick = PopGetPageItemClick
    end
    object N1: TMenuItem
      Tag = -1
      Caption = '-'
    end
    object PopGetPathItem: TMenuItem
      Tag = -1
      Caption = #12501#12523#12497#12473#20184#12501#12449#12452#12523#21517'(&P)'
      OnClick = PopGetPathItemClick
    end
    object PopGetFileItem: TMenuItem
      Tag = -1
      Caption = #12501#12449#12452#12523#21517' (&N)'
      OnClick = PopGetFileItemClick
    end
    object PopGetNameItem: TMenuItem
      Tag = -1
      Caption = #25313#24373#23376#12434#38500#12356#12383#12501#12449#12452#12523#21517' (&M)'
      OnClick = PopGetNameItemClick
    end
  end
  object OpenImgDlg: TOpenPictureDialog
    Filter = 
      #12377#12409#12390#12398#12501#12449#12452#12523' (*.bmp;*.ico;*.emf;*.wmf)|*.bmp;*.ico;*.emf;*.wmf|'#12499#12483#12488#12510#12483 +
      #12503' (*.bmp)|*.bmp|'#12450#12452#12467#12531' (*.ico)|*.ico|'#12456#12531#12495#12531#12473' '#12513#12479#12501#12449#12452#12523' (*.emf)|*.emf|'#12513#12479 +
      #12501#12449#12452#12523' (*.wmf)|*.wmf'
    Left = 104
    Top = 64
  end
  object ActionList1: TActionList
    Left = 104
    Top = 112
    object OkAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = '&OK'
      HelpType = htContext
      Hint = #12503#12525#12497#12486#12451#30906#23450'(OK)'
      OnExecute = OkActionExecute
      OnUpdate = OkActionUpdate
    end
    object TypeFrgAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = #26029#29255
      HelpType = htContext
      Hint = #12479#12452#12503' : '#26029#29255
      OnExecute = TypeFrgActionExecute
      OnUpdate = TypeFrgActionUpdate
    end
    object TypePltAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = #34920#26413
      HelpType = htContext
      Hint = #12479#12452#12503' : '#34920#26413
      OnExecute = TypePltActionExecute
      OnUpdate = TypePltActionUpdate
    end
    object TypeSymAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = #27161#35672
      HelpType = htContext
      Hint = #12479#12452#12503' : '#27161#35672
      OnExecute = TypeSymActionExecute
      OnUpdate = TypeSymActionUpdate
    end
    object TypeImgAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = #30011#20687
      HelpType = htContext
      Hint = #12479#12452#12503' : '#30011#20687
      OnExecute = TypeImgActionExecute
      OnUpdate = TypeImgActionUpdate
    end
    object FitWdAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = 'F&it'
      HelpType = htContext
      Hint = #26689#25968#12501#12451#12483#12488
      OnExecute = FitWdActionExecute
      OnUpdate = FitWdActionUpdate
    end
    object FitHiAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = '&Fit'
      HelpType = htContext
      Hint = #34892#25968#12501#12451#12483#12488
      OnExecute = FitHiActionExecute
      OnUpdate = FitHiActionUpdate
    end
    object CharColAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = #25991#23383
      HelpType = htContext
      Hint = #25991#23383#33394#12398#35373#23450
      OnExecute = CharColActionExecute
      OnUpdate = CharColActionUpdate
    end
    object BackColAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = #32972#26223
      HelpType = htContext
      Hint = #32972#26223#33394#12398#35373#23450
      OnExecute = BackColActionExecute
      OnUpdate = BackColActionUpdate
    end
    object RefLinkAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = '...'
      HelpType = htContext
      Hint = #12522#12531#12463#20808#12398#21442#29031
      OnExecute = RefLinkActionExecute
      OnUpdate = RefLinkActionUpdate
    end
    object GetLnkInfAction: TAction
      Category = '<'#12503#12525#12497#12486#12451#12539#12480#12452#12450#12525#12464'>'
      Caption = #9660
      HelpType = htContext
      Hint = #12522#12531#12463#20808#12501#12449#12452#12523#12398#24773#22577#12434#21462#24471
      OnExecute = GetLnkInfActionExecute
      OnUpdate = GetLnkInfActionUpdate
    end
  end
end
