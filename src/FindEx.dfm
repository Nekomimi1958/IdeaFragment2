object FindExForm: TFindExForm
  Left = 0
  Top = 0
  HelpContext = 35
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = #25313#24373#26908#32034
  ClientHeight = 356
  ClientWidth = 452
  Color = clBtnFace
  Constraints.MinHeight = 200
  Constraints.MinWidth = 460
  ParentFont = True
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  Scaled = False
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 452
    Height = 107
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    TabStop = True
    DesignSize = (
      452
      107)
    object Label1: TLabel
      Left = 34
      Top = 43
      Width = 40
      Height = 13
      Alignment = taRightJustify
      Caption = #23550#35937'(&O)'
      FocusControl = ObjModeComboBox
    end
    object Label2: TLabel
      Left = 24
      Top = 8
      Width = 50
      Height = 13
      Alignment = taRightJustify
      Caption = #26908#32034#35486'(&S)'
      FocusControl = FindComboBox
    end
    object Label3: TLabel
      Left = 24
      Top = 75
      Width = 50
      Height = 13
      Alignment = taRightJustify
      Caption = #12501#12457#12523#12480'(&D)'
      FocusControl = ObjDirComboBox
    end
    object Bevel1: TBevel
      Left = 0
      Top = 103
      Width = 452
      Height = 4
      Align = alBottom
      Shape = bsBottomLine
    end
    object FindComboBox: TComboBox
      Left = 76
      Top = 5
      Width = 179
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      DropDownCount = 20
      MaxLength = 80
      PopupMenu = SrcPopupMenu
      TabOrder = 0
      OnChange = ClearStatusMsg
    end
    object StartBtn: TButton
      Left = 387
      Top = 70
      Width = 60
      Height = 26
      Action = StartAction
      Anchors = [akTop, akRight]
      Default = True
      ModalResult = 1
      TabOrder = 1
    end
    object ObjModeComboBox: TComboBox
      Left = 76
      Top = 39
      Width = 179
      Height = 21
      Style = csDropDownList
      TabOrder = 2
      OnChange = ClearStatusMsg
    end
    object ObjDirComboBox: TComboBox
      Left = 76
      Top = 71
      Width = 146
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 3
      OnChange = ClearStatusMsg
    end
    object RefDirBtn: TButton
      Left = 223
      Top = 71
      Width = 32
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 4
      OnClick = RefDirBtnClick
    end
    object ExeModeComboBox: TComboBox
      Left = 271
      Top = 71
      Width = 107
      Height = 21
      Style = csDropDownList
      Anchors = [akTop, akRight]
      TabOrder = 5
      OnChange = ClearStatusMsg
    end
    object FindModeComboBox: TComboBox
      Left = 271
      Top = 5
      Width = 176
      Height = 21
      Style = csDropDownList
      Anchors = [akTop, akRight]
      TabOrder = 6
      OnChange = ClearStatusMsg
    end
    object CaseChkBox: TCheckBox
      Left = 279
      Top = 42
      Width = 168
      Height = 17
      Anchors = [akTop, akRight]
      Caption = #22823#23567#25991#23383#21306#21029'(&C)'
      TabOrder = 7
    end
    object HideBtn: TButton
      Left = 452
      Top = 0
      Width = 0
      Height = 103
      Align = alRight
      Cancel = True
      TabOrder = 8
      OnClick = HideBtnClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 107
    Width = 452
    Height = 226
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    TabStop = True
    object ResultGrid: TStringGrid
      Left = 0
      Top = 0
      Width = 452
      Height = 226
      Align = alClient
      BorderStyle = bsNone
      ColCount = 8
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect, goThumbTracking]
      PopupMenu = PopupMenu1
      TabOrder = 0
      StyleElements = [seBorder]
      OnClick = ResultGridClick
      OnDblClick = ResultGridDblClick
      OnDrawCell = ResultGridDrawCell
      OnKeyDown = ResultGridKeyDown
      ColWidths = (
        64
        64
        64
        64
        64
        64
        64
        64)
      RowHeights = (
        24
        24
        24
        24
        24)
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 333
    Width = 452
    Height = 23
    Panels = <
      item
        Width = 100
      end
      item
        Width = 200
      end
      item
        Width = 50
      end>
  end
  object ActionList1: TActionList
    Left = 392
    Top = 249
    object StartAction: TAction
      Caption = #38283#22987
      HelpType = htContext
      OnExecute = StartActionExecute
      OnUpdate = StartActionUpdate
    end
    object CopyListAction: TAction
      Caption = #12522#12473#12488#12398#26029#29255#12434#12467#12500#12540'(&C)'
      HelpType = htContext
      OnExecute = CopyListActionExecute
      OnUpdate = ListActionUpdate
    end
    object ClrListAction: TAction
      Caption = #12522#12473#12488#12434#12463#12522#12450'(&L)'
      HelpType = htContext
      OnExecute = ClrListActionExecute
      OnUpdate = ListActionUpdate
    end
    object CopyAnchorAction: TAction
      Caption = #12501#12449#12452#12523#21517'#ID'#30058#21495#12434#12467#12500#12540'(&I)'
      HelpType = htContext
      OnExecute = CopyAnchorActionExecute
      OnUpdate = ListSelOneUpdate
    end
    object PropertyAction: TAction
      Caption = #12503#12525#12497#12486#12451'(&R)...'
      HelpType = htContext
      OnExecute = PropertyActionExecute
      OnUpdate = ListSelOneUpdate
    end
    object JumpAction: TAction
      Caption = #12472#12515#12531#12503'(&J)'
      HelpType = htContext
      OnExecute = JumpActionExecute
      OnUpdate = ListSelOneUpdate
    end
    object SortFindHistAction: TAction
      Caption = #26908#32034#23653#27508#12434#12477#12540#12488'(&S)'
      OnExecute = SortFindHistActionExecute
      OnUpdate = SortFindHistActionUpdate
    end
    object DelFromHistAction: TAction
      Caption = #26908#32034#35486#12434#23653#27508#12363#12425#21066#38500
      OnExecute = DelFromHistActionExecute
      OnUpdate = DelFromHistActionUpdate
    end
    object ClrFindHistAction: TAction
      Caption = #26908#32034#23653#27508#12434#12377#12409#12390#21066#38500
      OnExecute = ClrFindHistActionExecute
      OnUpdate = ClrFindHistActionUpdate
    end
  end
  object PopupMenu1: TPopupMenu
    AutoHotkeys = maManual
    Left = 256
    Top = 249
    object QListCopyItem: TMenuItem
      Action = CopyListAction
    end
    object QListClrItem: TMenuItem
      Action = ClrListAction
    end
    object P_s1: TMenuItem
      Caption = '-'
    end
    object JumpItem: TMenuItem
      Action = JumpAction
    end
    object CopyAnchorItem: TMenuItem
      Action = CopyAnchorAction
    end
    object P_s2: TMenuItem
      Caption = '-'
    end
    object PropertyItem: TMenuItem
      Action = PropertyAction
    end
  end
  object SrcPopupMenu: TPopupMenu
    AutoHotkeys = maManual
    Left = 328
    Top = 249
    object SrcCutItem: TMenuItem
      Caption = #20999#12426#21462#12426'(&T)'
      Hint = #20999#12426#21462#12426'|'#36984#25246#37096#20998#12434#20999#12426#21462#12426#12289#12463#12522#12483#12503#12508#12540#12489#12395#36865#12427
      ImageIndex = 0
      ShortCut = 16472
      OnClick = SrcCutItemClick
    end
    object SrcCopyItem: TMenuItem
      Caption = #12467#12500#12540'(&C)'
      ShortCut = 16451
      OnClick = SrcCopyItemClick
    end
    object SrcPasteItem: TMenuItem
      Caption = #36028#12426#20184#12369'(&P)'
      ShortCut = 16470
      OnClick = SrcPasteItemClick
    end
    object Sep_src_1: TMenuItem
      Caption = '-'
    end
    object SrcSortHistItem: TMenuItem
      Action = SortFindHistAction
    end
    object SrcDelHistItem: TMenuItem
      Action = DelFromHistAction
    end
    object SrcClrHistItem: TMenuItem
      Action = ClrFindHistAction
    end
    object Sep_src_2: TMenuItem
      Caption = '-'
    end
    object RegExpItem: TMenuItem
      Caption = #27491#35215#34920#29694'(&R)'
      object PopReg001Item: TMenuItem
        Caption = '\     '#27425#12398#12513#12479#25991#23383#12434#12463#12457#12540#12488
        OnClick = RefRegItemClick
      end
      object PopReg002Item: TMenuItem
        Caption = '^     '#34892#38957
        OnClick = RefRegItemClick
      end
      object PopReg003Item: TMenuItem
        Caption = '.     ('#25913#34892#20197#22806#12398')'#12377#12409#12390#12398#25991#23383
        OnClick = RefRegItemClick
      end
      object PopReg004Item: TMenuItem
        Caption = '$     '#34892#26411
        OnClick = RefRegItemClick
      end
      object PopReg005Item: TMenuItem
        Caption = '|     '#36984#25246' '
        OnClick = RefRegItemClick
      end
      object PopReg006Item: TMenuItem
        Caption = '()    '#12464#12523#12540#12503#21270' '
        OnClick = RefRegItemClick
      end
      object PopReg007Item: TMenuItem
        Caption = '[]    '#25991#23383#12463#12521#12473' '
        OnClick = RefRegItemClick
      end
      object PopRegSep1Item1: TMenuItem
        Caption = '-'
        OnClick = RefRegItemClick
      end
      object PopReg010Item: TMenuItem
        Caption = '\w  '#12288#21336#35486#12398#27083#25104#25991#23383'('#33521#25968#23383#12392'"_")'
        OnClick = RefRegItemClick
      end
      object PopReg011Item: TMenuItem
        Caption = '\W    '#21336#35486#12398#27083#25104#25991#23383#20197#22806
        OnClick = RefRegItemClick
      end
      object PopReg012Item: TMenuItem
        Caption = '\s    '#31354#30333#25991#23383
        OnClick = RefRegItemClick
      end
      object PopReg013Item: TMenuItem
        Caption = '\S    '#31354#30333#25991#23383#20197#22806
        OnClick = RefRegItemClick
      end
      object PopReg014Item: TMenuItem
        Caption = '\d    '#25968#23383
        OnClick = RefRegItemClick
      end
      object PopReg015Item: TMenuItem
        Caption = '\D    '#25968#23383#20197#22806
        OnClick = RefRegItemClick
      end
      object PopReg016Item: TMenuItem
        Caption = '\b    '#21336#35486#12398#22659#30028
        OnClick = RefRegItemClick
      end
      object PopReg017Item: TMenuItem
        Caption = '\B    '#21336#35486#12398#22659#30028#20197#22806
        OnClick = RefRegItemClick
      end
      object PopReg018Item: TMenuItem
        Caption = '\A    '#25991#23383#21015#12398#26368#21021
        OnClick = RefRegItemClick
      end
      object PopReg019Item: TMenuItem
        Caption = '\Z    '#25991#23383#21015#12398#26368#24460
        OnClick = RefRegItemClick
      end
      object PopRegSep2Item1: TMenuItem
        Caption = '-'
        OnClick = RefRegItemClick
      end
      object PopReg020Item: TMenuItem
        Caption = '*      0 '#22238#20197#19978
        OnClick = RefRegItemClick
      end
      object PopReg021Item: TMenuItem
        Caption = '+      1 '#22238#20197#19978
        OnClick = RefRegItemClick
      end
      object PopReg022Item: TMenuItem
        Caption = '?      1 '#22238#12414#12383#12399' 0 '#22238
        OnClick = RefRegItemClick
      end
      object PopReg023Item: TMenuItem
        Caption = '{n}    '#12385#12423#12358#12393' n '#22238
        OnClick = RefRegItemClick
      end
      object PopReg024Item: TMenuItem
        Caption = '{n,}   n '#22238#20197#19978
        OnClick = RefRegItemClick
      end
      object PopReg025Item: TMenuItem
        Caption = '{n,m}  n '#22238#20197#19978' m '#22238#20197#19979
        OnClick = RefRegItemClick
      end
    end
  end
end
