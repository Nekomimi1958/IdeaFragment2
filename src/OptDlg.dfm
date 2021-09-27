object OptionDlg: TOptionDlg
  Left = 0
  Top = 0
  HelpContext = 12
  BorderStyle = bsDialog
  Caption = #12458#12503#12471#12519#12531#35373#23450
  ClientHeight = 511
  ClientWidth = 644
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  ShowHint = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 644
    Height = 475
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    ParentColor = True
    TabOrder = 0
    object PageControl1: TPageControl
      Left = 5
      Top = 5
      Width = 634
      Height = 465
      ActivePage = TabSheet6
      Align = alClient
      MultiLine = True
      OwnerDraw = True
      TabOrder = 0
      TabWidth = 120
      OnChange = PageControl1Change
      OnDrawTab = PageControl1DrawTab
      object TabSheet1: TTabSheet
        HelpContext = 13
        Caption = #12487#12470#12452#12531
        OnShow = TabSheet1Show
        object SamplePanel: TPanel
          Left = 8
          Top = 10
          Width = 249
          Height = 396
          BevelInner = bvLowered
          BevelWidth = 2
          Color = clWindow
          TabOrder = 0
          StyleElements = [seBorder]
          OnMouseDown = SamplePanelMouseDown
          object Splitter1: TSplitter
            Left = 4
            Top = 258
            Width = 241
            Height = 4
            Cursor = crVSplit
            Align = alBottom
            Beveled = True
            ResizeStyle = rsNone
          end
          object NrmFrgImage: TImage
            Left = 124
            Top = 36
            Width = 70
            Height = 42
            Enabled = False
          end
          object SelFrgImage: TImage
            Left = 84
            Top = 112
            Width = 70
            Height = 42
            Enabled = False
          end
          object PlateImage: TImage
            Left = 38
            Top = 32
            Width = 70
            Height = 70
            Enabled = False
          end
          object SymLabel: TLabel
            Left = 154
            Top = 184
            Width = 11
            Height = 13
            Caption = #9660
            OnClick = SymLabelClick
          end
          object GrpFrmShape: TShape
            Left = 20
            Top = 18
            Width = 201
            Height = 153
            Brush.Style = bsClear
            Enabled = False
            Shape = stRoundRect
          end
          object GLinePBox: TPaintBox
            Left = 28
            Top = 174
            Width = 120
            Height = 60
            OnMouseDown = GLinePBoxMouseDown
            OnPaint = GLinePBoxPaint
          end
          object FLinePBox: TPaintBox
            Left = 119
            Top = 80
            Width = 41
            Height = 30
            OnMouseDown = FLinePBoxMouseDown
            OnPaint = FLinePBoxPaint
          end
          object StatusBar1: TStatusBar
            Left = 4
            Top = 371
            Width = 241
            Height = 21
            Panels = <
              item
                Text = '1/1'
                Width = 50
              end
              item
                Text = #26029#29255#25968'=   2'
                Width = 50
              end>
            OnClick = StatusBar1Click
          end
          object GrpCheckListBox: TListBox
            Left = 4
            Top = 262
            Width = 241
            Height = 109
            TabStop = False
            Style = lbOwnerDrawFixed
            Align = alBottom
            Font.Charset = SHIFTJIS_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = #65325#65331' '#12468#12471#12483#12463
            Font.Style = []
            Items.Strings = (
              ' 10: '#12464#12523#12540#12503' 1'
              ' 20: '#12464#12523#12540#12503' 2'
              '  7: '#12464#12523#12540#12503' 3'
              '___: '#12464#12523#12540#12503' 4'
              ' 12: '#12464#12523#12540#12503' 5'
              '  5: '#12464#12523#12540#12503' 6'
              ' 24: '#12464#12523#12540#12503' 7'
              '___: '#12464#12523#12540#12503' 8'
              '___: '#12464#12523#12540#12503' 9')
            ParentFont = False
            TabOrder = 1
            OnClick = GrpCheckListBoxClick
            OnDrawItem = GrpCheckListBoxDrawItem
          end
        end
        object ColorGroupBox: TGroupBox
          Left = 271
          Top = 8
          Width = 343
          Height = 261
          Caption = #37197#33394'(&C)'
          TabOrder = 1
          DesignSize = (
            343
            261)
          object ColButton: TButton
            Left = 286
            Top = 18
            Width = 48
            Height = 24
            Caption = '...'
            TabOrder = 1
            OnClick = ColButtonClick
          end
          object DisColBtn: TButton
            Left = 286
            Top = 48
            Width = 48
            Height = 24
            Action = DisableColAction
            TabOrder = 2
          end
          object ColListBox: TListBox
            Left = 10
            Top = 18
            Width = 272
            Height = 232
            Style = lbOwnerDrawFixed
            Anchors = [akLeft, akTop, akBottom]
            TabOrder = 0
            OnDrawItem = ColListBoxDrawItem
          end
        end
        object FontGroupBox: TGroupBox
          Left = 271
          Top = 281
          Width = 343
          Height = 56
          Caption = #12501#12457#12531#12488'(&T)'
          TabOrder = 2
          DesignSize = (
            343
            56)
          object FontButton: TButton
            Left = 286
            Top = 21
            Width = 48
            Height = 24
            Caption = '...'
            Enabled = False
            TabOrder = 1
            OnClick = FontButtonClick
          end
          object FontComboBox: TComboBox
            Left = 10
            Top = 22
            Width = 272
            Height = 22
            Style = csOwnerDrawFixed
            Anchors = [akLeft, akTop, akRight]
            DropDownCount = 12
            TabOrder = 0
            OnChange = FontComboBoxChange
            OnDrawItem = FontComboBoxDrawItem
          end
        end
        object SizeGroupBox: TGroupBox
          Left = 271
          Top = 349
          Width = 343
          Height = 56
          Caption = #12469#12452#12474'(&S)'
          TabOrder = 3
          DesignSize = (
            343
            56)
          object SizValComboBox: TComboBox
            Left = 263
            Top = 22
            Width = 71
            Height = 21
            Style = csDropDownList
            TabOrder = 1
            OnChange = SizValComboBoxChange
          end
          object SizSpComboBox: TComboBox
            Left = 10
            Top = 22
            Width = 247
            Height = 21
            Style = csDropDownList
            Anchors = [akLeft, akTop, akRight]
            TabOrder = 0
            OnChange = SizSpComboBoxChange
          end
        end
      end
      object TabSheet2: TTabSheet
        HelpContext = 14
        Caption = #26029#29255
        object TLabel
          Left = 337
          Top = 23
          Width = 72
          Height = 13
          Alignment = taRightJustify
          Caption = #26032#35215#20316#25104#20301#32622
        end
        object TLabel
          Left = 353
          Top = 53
          Width = 56
          Height = 13
          Alignment = taRightJustify
          Caption = #29255#12389#12369#20301#32622
        end
        object TLabel
          Left = 325
          Top = 83
          Width = 84
          Height = 13
          Alignment = taRightJustify
          Caption = #31227#21205#26178#12398#12459#12540#12477#12523
        end
        object TLabel
          Left = 298
          Top = 313
          Width = 143
          Height = 13
          Caption = #30011#20687#12398#32302#23567#12539#25313#22823#12450#12523#12468#12522#12474#12512
        end
        object FragSizeGroupBox: TGroupBox
          Left = 10
          Top = 10
          Width = 245
          Height = 57
          Caption = #26029#29255#12487#12501#12457#12523#12488#12469#12452#12474'(&S)'
          TabOrder = 0
          object WdUpDown: TUpDown
            Left = 81
            Top = 24
            Width = 15
            Height = 21
            Associate = DefWdEdit
            Min = 4
            Max = 40
            Position = 12
            TabOrder = 0
            Thousands = False
          end
          object HiUpDown: TUpDown
            Left = 173
            Top = 24
            Width = 15
            Height = 21
            Associate = DefHiEdit
            Min = 1
            Max = 20
            Position = 3
            TabOrder = 1
            Thousands = False
          end
          object DefWdEdit: TLabeledEdit
            Left = 40
            Top = 24
            Width = 41
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 12
            EditLabel.Height = 13
            EditLabel.Caption = #26689
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 2
            Text = '12'
            OnChange = DefWdEditChange
            OnExit = NumberEditExit
          end
          object DefHiEdit: TLabeledEdit
            Left = 132
            Top = 24
            Width = 41
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 12
            EditLabel.Height = 13
            EditLabel.Caption = #34892
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 3
            Text = '3'
            OnChange = DefHiEditChange
            OnExit = NumberEditExit
          end
        end
        object ImeOnCheckBox: TCheckBox
          Left = 298
          Top = 211
          Width = 235
          Height = 17
          Caption = #12486#12461#12473#12488#20837#21147#26178'IME'#12458#12531
          TabOrder = 10
        end
        object PlateSizeGroupBox: TGroupBox
          Left = 10
          Top = 74
          Width = 245
          Height = 57
          Caption = #34920#26413#12487#12501#12457#12523#12488#12469#12452#12474'(&N)'
          TabOrder = 1
          object NpWdUpDown: TUpDown
            Left = 81
            Top = 24
            Width = 15
            Height = 21
            Associate = NamWdEdit
            Min = 4
            Max = 40
            Position = 10
            TabOrder = 1
            Thousands = False
          end
          object NpHiUpDown: TUpDown
            Left = 173
            Top = 24
            Width = 15
            Height = 21
            Associate = NamHiEdit
            Min = 1
            Max = 20
            Position = 3
            TabOrder = 3
            Thousands = False
          end
          object NamHiEdit: TLabeledEdit
            Left = 132
            Top = 24
            Width = 41
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 12
            EditLabel.Height = 13
            EditLabel.Caption = #34892
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 2
            Text = '3'
            OnChange = NamHiEditChange
            OnExit = NumberEditExit
          end
          object NamWdEdit: TLabeledEdit
            Left = 40
            Top = 24
            Width = 41
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 12
            EditLabel.Height = 13
            EditLabel.Caption = #26689
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 0
            Text = '10'
            OnChange = NamWdEditChange
            OnExit = NumberEditExit
          end
        end
        object ReturnGroupBox: TGroupBox
          Left = 10
          Top = 206
          Width = 245
          Height = 137
          Caption = #25913#34892'(&R)'
          TabOrder = 3
          object AcceptCrCheckBox: TCheckBox
            Left = 16
            Top = 24
            Width = 177
            Height = 17
            Caption = #65411#65399#65405#65412#12398#25913#34892#12434#34892#12358'(&R)'
            TabOrder = 0
            OnClick = AcceptCrCheckBoxClick
          end
          object CrRetKeyCheckBox: TCheckBox
            Left = 16
            Top = 48
            Width = 177
            Height = 17
            Caption = #25913#34892#12399'Return'#12461#12540#12391
            TabOrder = 1
          end
          object CtlRetOkCheckBox: TCheckBox
            Left = 16
            Top = 112
            Width = 177
            Height = 17
            Caption = 'Ctrl+Return'#12461#12540#12391'OK'
            TabOrder = 3
          end
          object CrStrEdit: TLabeledEdit
            Left = 128
            Top = 72
            Width = 65
            Height = 21
            EditLabel.Width = 60
            EditLabel.Height = 13
            EditLabel.Caption = #25913#34892#25991#23383#21015
            LabelPosition = lpLeft
            TabOrder = 2
          end
        end
        object AutoExpCheckBox: TCheckBox
          Left = 298
          Top = 161
          Width = 235
          Height = 17
          Caption = #33258#21205#30340#12395#34892#25968#22679
          TabOrder = 8
        end
        object MovCsrCombo: TComboBox
          Left = 416
          Top = 80
          Width = 80
          Height = 21
          Style = csDropDownList
          TabOrder = 6
          Items.Strings = (
            #30690#21360
            #24038#25163
            #21491#25163)
        end
        object NewPosCombo: TComboBox
          Left = 416
          Top = 20
          Width = 80
          Height = 21
          Style = csDropDownList
          TabOrder = 4
          Items.Strings = (
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
        object AwayPosCombo: TComboBox
          Left = 416
          Top = 50
          Width = 80
          Height = 21
          Style = csDropDownList
          TabOrder = 5
          Items.Strings = (
            #24038#19978#38533
            #21491#19978#38533
            #24038#19979#38533
            #21491#19979#38533)
        end
        object LnkIconCheckBox: TCheckBox
          Left = 298
          Top = 247
          Width = 235
          Height = 17
          Caption = #12522#12531#12463#12434#12450#12452#12467#12531#12391#34920#31034
          TabOrder = 11
        end
        object WdWrapCheckBox: TCheckBox
          Left = 298
          Top = 186
          Width = 235
          Height = 17
          Caption = #12486#12461#12473#12488#20837#21147#12398#21491#31471#12391#25240#12426#36820#12377
          TabOrder = 9
        end
        object NoGrpCheckBox: TCheckBox
          Left = 298
          Top = 125
          Width = 235
          Height = 17
          Caption = #26032#35215#20316#25104#26178#12395#28961#25152#23646
          TabOrder = 7
        end
        object ImageGroupBox: TGroupBox
          Left = 10
          Top = 138
          Width = 245
          Height = 57
          Caption = #30011#20687#12487#12501#12457#12523#12488#20493#29575'(&I)'
          TabOrder = 2
          object TLabel
            Left = 108
            Top = 28
            Width = 12
            Height = 13
            Alignment = taRightJustify
            Caption = #65295
          end
          object ImgR1UpDown: TUpDown
            Left = 81
            Top = 24
            Width = 17
            Height = 21
            Associate = ImgR1Edit
            Min = 1
            Max = 20
            Position = 1
            TabOrder = 1
            Thousands = False
          end
          object ImgR2UpDown: TUpDown
            Left = 171
            Top = 24
            Width = 17
            Height = 21
            Associate = ImgR2Edit
            Min = 1
            Max = 20
            Position = 1
            TabOrder = 3
            Thousands = False
          end
          object ImgR1Edit: TEdit
            Left = 40
            Top = 24
            Width = 41
            Height = 21
            Alignment = taRightJustify
            MaxLength = 2
            NumbersOnly = True
            TabOrder = 0
            Text = '1'
            OnChange = ImgR1EditChange
            OnExit = NumberEditExit
          end
          object ImgR2Edit: TEdit
            Left = 132
            Top = 24
            Width = 39
            Height = 21
            Alignment = taRightJustify
            MaxLength = 2
            NumbersOnly = True
            TabOrder = 2
            Text = '1'
            OnChange = ImgR2EditChange
            OnExit = NumberEditExit
          end
        end
        object LnkRelCheckBox: TCheckBox
          Left = 298
          Top = 272
          Width = 235
          Height = 17
          Caption = #12522#12531#12463#20808#12434#30456#23550#12497#12473#12391#34920#12377
          TabOrder = 12
        end
        object ScaleOptComboBox: TComboBox
          Left = 298
          Top = 333
          Width = 198
          Height = 21
          Style = csDropDownList
          TabOrder = 13
        end
      end
      object TabSheet3: TTabSheet
        HelpContext = 15
        Caption = #12471#12519#12540#12488#12459#12483#12488
        object KeyGroupBox: TGroupBox
          Left = 504
          Top = 8
          Width = 111
          Height = 393
          Caption = #12461#12540'(&K)'
          TabOrder = 2
          object KeyComboBox: TComboBox
            Left = 12
            Top = 24
            Width = 87
            Height = 21
            Style = csDropDownList
            DropDownCount = 12
            ImeMode = imClose
            TabOrder = 0
          end
          object CtrlCheckBox: TCheckBox
            Left = 18
            Top = 64
            Width = 65
            Height = 17
            Caption = 'Ctrl'
            TabOrder = 1
          end
          object ShiftCheckBox: TCheckBox
            Left = 18
            Top = 88
            Width = 65
            Height = 17
            Caption = 'Shift'
            TabOrder = 2
          end
          object SKChgBtn: TButton
            Left = 20
            Top = 200
            Width = 73
            Height = 24
            Caption = #22793#26356'(&C)'
            TabOrder = 4
            OnClick = SKChgBtnClick
          end
          object ClrBtn: TButton
            Left = 20
            Top = 240
            Width = 73
            Height = 24
            Caption = #35299#38500'(&R)'
            TabOrder = 5
            OnClick = ClrBtnClick
          end
          object AltCheckBox: TCheckBox
            Left = 18
            Top = 112
            Width = 65
            Height = 17
            Caption = 'Alt'
            TabOrder = 3
          end
        end
        object ShortCutList: TListBox
          Left = 10
          Top = 42
          Width = 480
          Height = 359
          Style = lbOwnerDrawFixed
          ItemHeight = 13
          TabOrder = 1
          TabWidth = 120
          OnClick = ShortCutListClick
          OnDrawItem = ShortCutListDrawItem
        end
        object ParentCombo: TComboBox
          Left = 10
          Top = 16
          Width = 480
          Height = 21
          Style = csDropDownList
          DropDownCount = 10
          TabOrder = 0
          OnClick = ParentComboClick
        end
      end
      object TabSheet4: TTabSheet
        HelpContext = 16
        Caption = #12454#12451#12531#12489#12454
        ImageIndex = 3
        object HomeGroupBox: TGroupBox
          Left = 10
          Top = 10
          Width = 599
          Height = 97
          Caption = #12507#12540#12512#12454#12451#12531#12489#12454'(&H)'
          TabOrder = 0
          DesignSize = (
            599
            97)
          object HomeWinEdit: TEdit
            Left = 16
            Top = 24
            Width = 527
            Height = 21
            Anchors = [akLeft, akTop, akRight]
            ImeMode = imClose
            TabOrder = 0
          end
          object HomWinBtn: TButton
            Left = 546
            Top = 24
            Width = 35
            Height = 22
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 1
            OnClick = HomWinBtnClick
          end
          object HomWinMaxCheckBox: TCheckBox
            Left = 32
            Top = 66
            Width = 145
            Height = 17
            Caption = #26368#22823#21270#12375#12390#38283#12367
            TabOrder = 2
          end
          object SetCurHomBtn: TButton
            Left = 263
            Top = 62
            Width = 280
            Height = 24
            Anchors = [akLeft, akTop, akRight]
            Caption = #29694#22312#12398#12501#12449#12452#12523#12434#25351#23450
            TabOrder = 3
            OnClick = SetCurHomBtnClick
          end
        end
        object ChildMaxCheckBox: TCheckBox
          Left = 16
          Top = 208
          Width = 410
          Height = 17
          Caption = #65297#20491#30446#12398#12454#12451#12531#12489#12454#12399#26368#22823#21270#12377#12427'(&Z)'
          TabOrder = 2
        end
        object RstScrPosCheckBox: TCheckBox
          Left = 16
          Top = 258
          Width = 410
          Height = 17
          Caption = #12473#12463#12525#12540#12523#12496#12540#12434#21069#22238#12398#20301#32622#12395#24489#20803#12377#12427'(&P)'
          TabOrder = 4
        end
        object HideTitleCheckBox: TCheckBox
          Left = 16
          Top = 233
          Width = 410
          Height = 17
          Caption = #26368#22823#21270#26178#12395#12479#12452#12488#12523#12496#12540#12392#12513#12491#12517#12540#12434#38560#12377'(&H)'
          TabOrder = 3
        end
        object ActDstWinCheckBox: TCheckBox
          Left = 16
          Top = 283
          Width = 410
          Height = 17
          Caption = #26029#29255#12398#31227#21205#20808#12454#12451#12531#12489#12454#12434#12450#12463#12486#12451#12502#12395'(&M)'
          TabOrder = 5
        end
        object OpenLastCheckBox: TCheckBox
          Left = 16
          Top = 308
          Width = 410
          Height = 17
          Caption = #36215#21205#26178#12395#21069#22238#12450#12463#12486#12451#12502#12384#12387#12383#12501#12449#12452#12523#12434#38283#12367'(&L)'
          TabOrder = 6
        end
        object ScrSizeGroupBox: TGroupBox
          Left = 10
          Top = 120
          Width = 255
          Height = 60
          Caption = #30011#38754#12487#12501#12457#12523#12488#12469#12452#12474'(&S)'
          TabOrder = 1
          object ScrWdUpDown: TUpDown
            Left = 93
            Top = 25
            Width = 15
            Height = 21
            Associate = ScrWdEdit
            Min = 100
            Max = 30000
            Position = 800
            TabOrder = 0
            Thousands = False
          end
          object ScrHiUpDown: TUpDown
            Left = 221
            Top = 25
            Width = 15
            Height = 21
            Associate = ScrHiEdit
            Min = 100
            Max = 30000
            Position = 600
            TabOrder = 1
            Thousands = False
          end
          object ScrWdEdit: TLabeledEdit
            Left = 36
            Top = 25
            Width = 57
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 12
            EditLabel.Height = 13
            EditLabel.Caption = #24133
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 2
            Text = '800'
            OnChange = ScrWdEditChange
            OnExit = NumberEditExit
          end
          object ScrHiEdit: TLabeledEdit
            Left = 164
            Top = 25
            Width = 57
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 20
            EditLabel.Height = 13
            EditLabel.Caption = #39640#12373
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 3
            Text = '600'
            OnChange = ScrHiEditChange
            OnExit = NumberEditExit
          end
        end
      end
      object TabSheet5: TTabSheet
        HelpContext = 17
        Caption = #34920#31034
        ImageIndex = 4
        object TLabel
          Left = 50
          Top = 121
          Width = 24
          Height = 13
          Alignment = taRightJustify
          Caption = #20301#32622
        end
        object TLabel
          Left = 229
          Top = 121
          Width = 12
          Height = 13
          Alignment = taRightJustify
          Caption = #24418
        end
        object NoNpHideGfrmCheckBox: TCheckBox
          Left = 16
          Top = 66
          Width = 410
          Height = 17
          Caption = #34920#26413#12364#12394#12356#22580#21512#12399#12464#12523#12540#12503#26528#12434#34920#31034#12375#12394#12356
          TabOrder = 2
        end
        object ShowDstGrpCheckBox: TCheckBox
          Left = 16
          Top = 214
          Width = 410
          Height = 17
          Caption = #26029#29255#31227#21205#20808#12398#12464#12523#12540#12503#12364#38750#34920#31034#12384#12387#12383#12425#34920#31034#12395
          TabOrder = 8
        end
        object GlRightAngCheckBox: TCheckBox
          Left = 16
          Top = 164
          Width = 410
          Height = 17
          Caption = #36794#21516#22763#12434#32080#12406#38306#20418#32218#12434#12394#12427#12409#12367#22402#30452#12395
          TabOrder = 6
        end
        object OnlyNpHideGfrmCheckBox: TCheckBox
          Left = 16
          Top = 41
          Width = 410
          Height = 17
          Caption = #34920#26413#12384#12369#12398#22580#21512#12399#12464#12523#12540#12503#26528#12434#34920#31034#12375#12394#12356
          TabOrder = 1
        end
        object AlwaysShowCheckBox: TCheckBox
          Left = 16
          Top = 16
          Width = 410
          Height = 17
          Caption = #34920#26413#12434#24120#12395#34920#31034#12377#12427
          TabOrder = 0
        end
        object ImgIsBakCheckBox: TCheckBox
          Left = 16
          Top = 189
          Width = 410
          Height = 17
          Caption = #30011#20687#26029#29255#12399#32972#24460#12395#34920#31034#12377#12427
          TabOrder = 7
        end
        object PlateOnGfrmCheckBox: TCheckBox
          Left = 16
          Top = 96
          Width = 289
          Height = 17
          Caption = #34920#26413#12434#12464#12523#12540#12503#26528#19978#12395#34920#31034#12377#12427
          TabOrder = 3
        end
        object PlateAlignComboBox: TComboBox
          Left = 78
          Top = 117
          Width = 129
          Height = 21
          Style = csDropDownList
          TabOrder = 4
        end
        object PlateShapeComboBox: TComboBox
          Left = 245
          Top = 117
          Width = 81
          Height = 21
          Style = csDropDownList
          TabOrder = 5
        end
        object MapVSelRctCheckBox: TCheckBox
          Left = 16
          Top = 262
          Width = 410
          Height = 17
          Caption = #20840#20307#22259#12395#36984#25246#26528#12434#34920#31034#12377#12427
          TabOrder = 9
        end
        object MapAlphaEdit: TLabeledEdit
          Left = 116
          Top = 312
          Width = 50
          Height = 21
          Alignment = taRightJustify
          EditLabel.Width = 82
          EditLabel.Height = 13
          EditLabel.Caption = #20840#20307#22259#12398#36879#26126#24230
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 11
        end
        object FitMapVCheckBox: TCheckBox
          Left = 16
          Top = 287
          Width = 410
          Height = 17
          Caption = #20840#20307#22259#12398#12454#12451#12531#12489#12454#12469#12452#12474#12434#33258#21205#12501#12451#12483#12488
          TabOrder = 10
        end
      end
      object TabSheet6: TTabSheet
        HelpContext = 18
        Caption = #12381#12398#20182'1'
        ImageIndex = 5
        object TLabel
          Left = 16
          Top = 17
          Width = 58
          Height = 13
          Caption = #26029#29255#12398#23637#38283
        end
        object JpgQLabel: TLabel
          Left = 360
          Top = 339
          Width = 93
          Height = 13
          Alignment = taRightJustify
          Caption = 'JPEG'#20445#23384#21697#36074' 100'
        end
        object TLabel
          Left = 16
          Top = 48
          Width = 109
          Height = 13
          Caption = #26029#29255#38598#21512#26178#12398#12378#12425#12375#24133
        end
        object NewBlkDblClickCheckBox: TCheckBox
          Left = 16
          Top = 260
          Width = 317
          Height = 17
          Caption = #12464#12523#12540#12503#12522#12473#12488#12398#31354#12464#12523#12540#12503#12434#12480#12502#12523#12463#12522#12483#12463#12391#26032#35215#26029#29255
          TabOrder = 9
        end
        object FrmAllSelCheckBox: TCheckBox
          Left = 16
          Top = 285
          Width = 317
          Height = 17
          Caption = #26528#36984#25246#12398#38555#12395#20132#24046#12375#12383#26029#29255#12399#12377#12409#12390#21547#12417#12427
          TabOrder = 10
        end
        object PosSortCheckBox: TCheckBox
          Left = 16
          Top = 185
          Width = 317
          Height = 17
          Caption = #12467#12500#12540#12362#12424#12403#12456#12463#12473#12509#12540#12488#26178#12395#26029#29255#20301#32622#12391#12477#12540#12488
          TabOrder = 6
        end
        object AlignCheckBox: TCheckBox
          Left = 97
          Top = 16
          Width = 126
          Height = 17
          Caption = #25539#12360#12390#20006#12409#12427'(&I)'
          TabOrder = 0
        end
        object ShuffleCheckBox: TCheckBox
          Left = 224
          Top = 16
          Width = 113
          Height = 17
          Caption = #12471#12515#12483#12501#12523'(&S)'
          TabOrder = 1
        end
        object DropImgCheckBox: TCheckBox
          Left = 16
          Top = 235
          Width = 317
          Height = 17
          Caption = #30011#20687#12501#12449#12452#12523#12489#12525#12483#12503#26178#12395#30011#20687#26029#29255#20316#25104
          TabOrder = 8
        end
        object JpgQTrackBar: TTrackBar
          Left = 458
          Top = 335
          Width = 150
          Height = 33
          Max = 100
          Min = 10
          Frequency = 10
          Position = 10
          TabOrder = 16
          OnChange = JpgQTrackBarChange
        end
        object ArngPtUpDown: TUpDown
          Left = 180
          Top = 45
          Width = 17
          Height = 21
          Associate = ArngPtEdit
          Min = 1
          Max = 50
          Position = 1
          TabOrder = 3
        end
        object ChgGrpModeCtrlCheckBox: TCheckBox
          Left = 16
          Top = 310
          Width = 317
          Height = 17
          Caption = #12464#12523#12540#12503#21270#26178#12398'Ctrl'#12461#12540#12398#24441#21106#12434#20837#12428#26367#12360#12427
          TabOrder = 11
        end
        object GridGroupBox: TGroupBox
          Left = 16
          Top = 82
          Width = 240
          Height = 57
          Caption = #37197#32622#12464#12522#12483#12489'(&G)'
          TabOrder = 4
          object GridSzUpDown: TUpDown
            Left = 84
            Top = 24
            Width = 15
            Height = 21
            Associate = GridSzEdit
            Min = 4
            Max = 99
            Position = 8
            TabOrder = 1
          end
          object SnapCheckBox: TCheckBox
            Left = 136
            Top = 26
            Width = 81
            Height = 17
            Caption = #12473#12490#12483#12503
            TabOrder = 2
          end
          object GridSzEdit: TLabeledEdit
            Left = 51
            Top = 24
            Width = 33
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 29
            EditLabel.Height = 13
            EditLabel.Caption = #12469#12452#12474
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 0
            Text = '8'
            OnChange = GridSzEditChange
            OnExit = NumberEditExit
          end
        end
        object SelJumpFrgCheckBox: TCheckBox
          Left = 16
          Top = 360
          Width = 317
          Height = 17
          Caption = #12472#12515#12531#12503#20808#12398#26029#29255#12434#36984#25246#29366#24907#12395#12377#12427
          TabOrder = 13
        end
        object CopyCLineCheckBox: TCheckBox
          Left = 16
          Top = 160
          Width = 317
          Height = 17
          Caption = #20999#12426#21462#12426#12539#12467#12500#12540#12391#38306#20418#32218#12418#21547#12417#12427
          TabOrder = 5
        end
        object ArngPtEdit: TEdit
          Left = 147
          Top = 45
          Width = 33
          Height = 21
          Alignment = taRightJustify
          MaxLength = 2
          NumbersOnly = True
          TabOrder = 2
          Text = '1'
          OnChange = ArngPtEditChange
          OnExit = NumberEditExit
        end
        object PaintGroupBox: TGroupBox
          Left = 343
          Top = 109
          Width = 268
          Height = 176
          Caption = #21360#21047
          TabOrder = 15
          object TLabel
            Left = 32
            Top = 138
            Width = 72
            Height = 13
            Alignment = taRightJustify
            Caption = #26368#23567#21360#21047#32218#24133
          end
          object PrintMonoCheckBox: TCheckBox
            Left = 12
            Top = 22
            Width = 177
            Height = 17
            Caption = #12514#12494#12463#12525'(1'#33394')'#12391#21360#21047
            TabOrder = 0
            OnClick = PrintMonoCheckBoxClick
          end
          object PrintBgCheckBox: TCheckBox
            Left = 12
            Top = 47
            Width = 177
            Height = 17
            Caption = #30011#38754#32972#26223#33394#12434#21360#21047
            TabOrder = 1
          end
          object PrintTimeCheckBox: TCheckBox
            Left = 12
            Top = 72
            Width = 177
            Height = 17
            Caption = #21360#21047#26085#26178#12434#21360#23383
            TabOrder = 2
          end
          object PrintScrSizeCheckBox: TCheckBox
            Left = 12
            Top = 97
            Width = 252
            Height = 17
            Caption = #20491#21029#35373#23450#12398#30011#38754#12469#12452#12474#12395#22522#12389#12356#12390#21360#21047
            TabOrder = 3
          end
          object MinPrnLWCombo: TComboBox
            Left = 109
            Top = 135
            Width = 120
            Height = 21
            Style = csDropDownList
            TabOrder = 4
          end
        end
        object NewDropPosCheckBox: TCheckBox
          Left = 16
          Top = 210
          Width = 317
          Height = 17
          Caption = #12489#12525#12483#12503#20301#32622#12395#26029#29255#20316#25104
          TabOrder = 7
        end
        object WheelGroupBox: TGroupBox
          Left = 343
          Top = 8
          Width = 268
          Height = 85
          Caption = #12510#12454#12473#12507#12452#12540#12523#25805#20316
          TabOrder = 14
          object TLabel
            Left = 132
            Top = 23
            Width = 12
            Height = 13
            Caption = 'px'
          end
          object ScrQtyEdit: TLabeledEdit
            Left = 88
            Top = 20
            Width = 40
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 58
            EditLabel.Height = 13
            EditLabel.Caption = #12473#12463#12525#12540#12523#37327
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 0
          end
          object ShiftHorzCheckBox: TCheckBox
            Left = 12
            Top = 53
            Width = 217
            Height = 17
            Caption = 'Shift + '#12507#12452#12540#12523#12391#27178#12473#12463#12525#12540#12523
            TabOrder = 1
          end
        end
        object RDragScrollCheckBox: TCheckBox
          Left = 16
          Top = 335
          Width = 317
          Height = 17
          Caption = #21491#12489#12521#12483#12464#12391#30011#38754#12434#12473#12463#12525#12540#12523#12377#12427
          TabOrder = 12
        end
      end
      object TabSheet9: TTabSheet
        HelpContext = 19
        Caption = #12381#12398#20182'2'
        ImageIndex = 8
        DesignSize = (
          626
          419)
        object TrimGroupBox: TGroupBox
          Left = 10
          Top = 8
          Width = 561
          Height = 141
          Caption = #12488#12522#12511#12531#12464'(&T)'
          TabOrder = 0
          object TrmSizCheckBox: TCheckBox
            Left = 16
            Top = 58
            Width = 193
            Height = 17
            Caption = #30011#38754#12469#12452#12474#12434#22793#26356#12377#12427
            TabOrder = 3
          end
          object TrmMgnUpDown: TUpDown
            Left = 337
            Top = 21
            Width = 15
            Height = 21
            Associate = TrmMgnEdit
            Max = 1000
            Position = 800
            TabOrder = 2
            Thousands = False
          end
          object TrmMovCheckBox: TCheckBox
            Left = 16
            Top = 24
            Width = 193
            Height = 17
            Caption = #30011#38754#24038#19978#31471#12395#31227#21205#12377#12427
            TabOrder = 0
          end
          object TrmFixCheckBox: TCheckBox
            Left = 40
            Top = 80
            Width = 169
            Height = 17
            Caption = #22793#26356#12469#12452#12474#22266#23450
            TabOrder = 4
          end
          object TrmWdUpDown: TUpDown
            Left = 337
            Top = 76
            Width = 15
            Height = 21
            Associate = TrmWdEdit
            Min = 100
            Max = 30000
            Position = 800
            TabOrder = 6
            Thousands = False
          end
          object TrmHiUpDown: TUpDown
            Left = 464
            Top = 76
            Width = 15
            Height = 21
            Associate = TrmHiEdit
            Min = 100
            Max = 30000
            Position = 600
            TabOrder = 8
            Thousands = False
          end
          object TrmSavCheckBox: TCheckBox
            Left = 16
            Top = 114
            Width = 209
            Height = 17
            Caption = #20966#29702#24460#12395#21517#21069#12434#20184#12369#12390#20445#23384
            TabOrder = 9
          end
          object TrmMgnEdit: TLabeledEdit
            Left = 280
            Top = 21
            Width = 57
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 24
            EditLabel.Height = 13
            EditLabel.Caption = #20313#30333
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 1
            Text = '800'
            OnChange = TrmMgnEditChange
            OnExit = NumberEditExit
          end
          object TrmWdEdit: TLabeledEdit
            Left = 280
            Top = 76
            Width = 57
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 12
            EditLabel.Height = 13
            EditLabel.Caption = #24133
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 5
            Text = '800'
            OnChange = TrmWdEditChange
            OnExit = NumberEditExit
          end
          object TrmHiEdit: TLabeledEdit
            Left = 407
            Top = 76
            Width = 57
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 20
            EditLabel.Height = 13
            EditLabel.Caption = #39640#12373
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 7
            Text = '600'
            OnChange = TrmHiEditChange
            OnExit = NumberEditExit
          end
        end
        object RegIfdGroupBox: TGroupBox
          Left = 347
          Top = 194
          Width = 224
          Height = 88
          Caption = #12487#12540#12479#12501#12449#12452#12523'(.IFD)'#12398#38306#36899#12389#12369
          TabOrder = 3
          object AdminLabel: TLabel
            Left = 19
            Top = 58
            Width = 3
            Height = 13
          end
          object RegExtBtn: TButton
            Left = 19
            Top = 26
            Width = 75
            Height = 24
            Caption = #35373#23450
            TabOrder = 0
            OnClick = RegExtBtnClick
          end
          object DelExtBtn: TButton
            Left = 123
            Top = 26
            Width = 75
            Height = 24
            Caption = #35299#38500
            TabOrder = 1
            OnClick = DelExtBtnClick
          end
        end
        object ExportGroupBox: TGroupBox
          Left = 10
          Top = 194
          Width = 325
          Height = 213
          Anchors = [akLeft, akTop, akBottom]
          Caption = #12456#12463#12473#12509#12540#12488#38917#30446
          TabOrder = 2
          DesignSize = (
            325
            213)
          object ExpItemListBox1: TListBox
            Left = 12
            Top = 24
            Width = 110
            Height = 178
            Anchors = [akLeft, akTop, akBottom]
            ItemHeight = 13
            MultiSelect = True
            TabOrder = 0
          end
          object ExpItemListBox2: TListBox
            Left = 203
            Top = 24
            Width = 110
            Height = 178
            Anchors = [akLeft, akTop, akBottom]
            ItemHeight = 13
            MultiSelect = True
            TabOrder = 5
          end
          object AddExpItemBtn: TButton
            Left = 132
            Top = 32
            Width = 61
            Height = 24
            Action = AddExpItemAction
            TabOrder = 1
          end
          object DelExpItemBtn: TButton
            Left = 132
            Top = 64
            Width = 61
            Height = 24
            Action = DelExpItemAction
            TabOrder = 2
          end
          object UpExpItemBtn: TButton
            Left = 159
            Top = 108
            Width = 34
            Height = 24
            Action = UpExpItemAction
            TabOrder = 3
          end
          object DowExpItemBtn: TButton
            Left = 159
            Top = 138
            Width = 34
            Height = 24
            Action = DowExpItemAction
            TabOrder = 4
          end
        end
        object ExportUtf8CheckBox: TCheckBox
          Left = 15
          Top = 165
          Width = 284
          Height = 17
          Caption = 'Unicode(UTF-8)'#12391#12456#12463#12473#12509#12540#12488
          TabOrder = 1
        end
      end
      object TabSheet7: TTabSheet
        HelpContext = 20
        Caption = #12503#12521#12464#12452#12531
        ImageIndex = 6
        object SusieGroupBox: TGroupBox
          Left = 10
          Top = 8
          Width = 603
          Height = 393
          Caption = 'Susie'#12503#12521#12464#12452#12531'(&S)'
          TabOrder = 0
          DesignSize = (
            603
            393)
          object SpiListBox: TListBox
            Left = 10
            Top = 56
            Width = 581
            Height = 289
            Anchors = [akLeft, akTop, akRight, akBottom]
            ItemHeight = 13
            TabOrder = 2
          end
          object SpiRefBtn: TButton
            Left = 546
            Top = 25
            Width = 41
            Height = 22
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 1
            OnClick = SpiRefBtnClick
          end
          object ConfSpiBtn: TButton
            Left = 512
            Top = 357
            Width = 75
            Height = 24
            Action = ConfigSpiAction
            Anchors = [akRight, akBottom]
            TabOrder = 4
          end
          object UseSpiFirstCheckBox: TCheckBox
            Left = 20
            Top = 360
            Width = 225
            Height = 17
            Anchors = [akLeft, akBottom]
            Caption = #12503#12521#12464#12452#12531#12434#20778#20808#30340#12395#20351#29992#12377#12427
            TabOrder = 3
          end
          object SpiDirEdit: TLabeledEdit
            Left = 81
            Top = 25
            Width = 463
            Height = 21
            EditLabel.Width = 49
            EditLabel.Height = 13
            EditLabel.Caption = #12487#12451#12524#12463#12488#12522
            LabelPosition = lpLeft
            TabOrder = 0
          end
        end
      end
      object TabSheet8: TTabSheet
        HelpContext = 21
        Caption = #38306#36899#20184#12369
        ImageIndex = 7
        object TLabel
          Left = 121
          Top = 343
          Width = 11
          Height = 13
          Caption = #8594
        end
        object AssociateListBox: TListBox
          Left = 10
          Top = 10
          Width = 603
          Height = 313
          ItemHeight = 13
          TabOrder = 0
          OnClick = AssociateListBoxClick
        end
        object AsoAddBtn: TButton
          Left = 24
          Top = 380
          Width = 75
          Height = 24
          Caption = #36861#21152
          TabOrder = 4
          OnClick = AsoAddBtnClick
        end
        object AsoAppEdit: TEdit
          Left = 144
          Top = 340
          Width = 424
          Height = 21
          TabOrder = 2
        end
        object AsoRefBtn: TButton
          Left = 571
          Top = 340
          Width = 42
          Height = 22
          Caption = '...'
          TabOrder = 3
          OnClick = AsoRefBtnClick
        end
        object AsoChgBtn: TButton
          Left = 112
          Top = 380
          Width = 75
          Height = 24
          Caption = #22793#26356
          TabOrder = 5
          OnClick = AsoChgBtnClick
        end
        object AsoDelBtn: TButton
          Left = 200
          Top = 380
          Width = 75
          Height = 24
          Caption = #21066#38500
          TabOrder = 6
          OnClick = AsoDelBtnClick
        end
        object AsoExtEdit: TLabeledEdit
          Left = 64
          Top = 340
          Width = 50
          Height = 21
          EditLabel.Width = 36
          EditLabel.Height = 13
          EditLabel.Caption = #25313#24373#23376
          LabelPosition = lpLeft
          TabOrder = 1
        end
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 475
    Width = 644
    Height = 36
    HelpContext = 9
    Align = alBottom
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 1
    object OKBtn: TButton
      Left = 213
      Top = 3
      Width = 89
      Height = 26
      Action = OkAction
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object CancelBtn: TButton
      Left = 342
      Top = 3
      Width = 89
      Height = 26
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 1
    end
    object FindEdit: TLabeledEdit
      Left = 488
      Top = 4
      Width = 135
      Height = 21
      EditLabel.Width = 24
      EditLabel.Height = 13
      EditLabel.Caption = #26908#32034
      LabelPosition = lpLeft
      TabOrder = 2
      OnChange = FindEditChange
    end
  end
  object ColorDialog1: TColorDialog
    Left = 368
    Top = 421
  end
  object FontDialog1: TFontDialog
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Style = []
    Options = [fdAnsiOnly]
    Left = 432
    Top = 421
  end
  object OpenDialog1: TOpenDialog
    Left = 496
    Top = 421
  end
  object ActionList1: TActionList
    Left = 305
    Top = 421
    object OkAction: TAction
      Caption = 'OK'
      HelpType = htContext
      OnExecute = OkActionExecute
      OnUpdate = OkActionUpdate
    end
    object AddExpItemAction: TAction
      Caption = #36861#21152#8594
      HelpType = htContext
      OnExecute = AddExpItemActionExecute
      OnUpdate = AddExpItemActionUpdate
    end
    object DelExpItemAction: TAction
      Caption = #8592#35299#38500
      HelpType = htContext
      OnExecute = DelExpItemActionExecute
      OnUpdate = DelExpItemActionUpdate
    end
    object UpExpItemAction: TAction
      Caption = #8593
      HelpType = htContext
      OnExecute = UpExpItemActionExecute
      OnUpdate = UpExpItemActionUpdate
    end
    object DowExpItemAction: TAction
      Caption = #8595
      HelpType = htContext
      OnExecute = DowExpItemActionExecute
      OnUpdate = DowExpItemActionUpdate
    end
    object ConfigSpiAction: TAction
      Caption = #35373#23450
      OnExecute = ConfigSpiActionExecute
      OnUpdate = ConfigSpiActionUpdate
    end
    object DisableColAction: TAction
      Caption = #28961#21177
      OnExecute = DisableColActionExecute
      OnUpdate = DisableColActionUpdate
    end
  end
end
