object FindSetDlg: TFindSetDlg
  Left = 0
  Top = 0
  HelpContext = 6
  Anchors = []
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #26908#32034
  ClientHeight = 107
  ClientWidth = 354
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = False
  Position = poDesigned
  Scaled = False
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  DesignSize = (
    354
    107)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 18
    Top = 13
    Width = 50
    Height = 13
    Alignment = taRightJustify
    Caption = #26908#32034#35486'(&S)'
    FocusControl = FindComboBox
  end
  object OKBtn: TButton
    Left = 253
    Top = 74
    Width = 89
    Height = 26
    Action = StartAction
    Anchors = [akLeft, akBottom]
    Default = True
    ModalResult = 1
    TabOrder = 4
  end
  object FindComboBox: TComboBox
    Left = 72
    Top = 10
    Width = 270
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 20
    MaxLength = 80
    PopupMenu = SrcPopupMenu
    TabOrder = 0
  end
  object CaseChkBox: TCheckBox
    Left = 210
    Top = 44
    Width = 137
    Height = 17
    Caption = #22823#23567#25991#23383#21306#21029'(&C)'
    TabOrder = 2
  end
  object FlUpCheckBox: TCheckBox
    Left = 16
    Top = 78
    Width = 231
    Height = 17
    Caption = #26029#29255#12522#12473#12488#12391#20808#38957#12395#38598#12417#12427'(&T)'
    TabOrder = 3
  end
  object FindModeComboBox: TComboBox
    Left = 18
    Top = 42
    Width = 176
    Height = 21
    Style = csDropDownList
    Anchors = [akTop, akRight]
    TabOrder = 1
  end
  object CanButton: TButton
    Left = 240
    Top = 75
    Width = 0
    Height = 25
    Cancel = True
    ModalResult = 2
    TabOrder = 5
  end
  object SrcPopupMenu: TPopupMenu
    AutoHotkeys = maManual
    Left = 288
    Top = 1
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
    object N1: TMenuItem
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
  object ActionList1: TActionList
    Left = 216
    object SortFindHistAction: TAction
      Caption = #26908#32034#23653#27508#12434#12477#12540#12488'(&S)'
      OnExecute = SortFindHistActionExecute
      OnUpdate = SortFindHistActionUpdate
    end
    object DelFromHistAction: TAction
      Caption = #26908#32034#35486#12434#23653#27508#12363#12425#21066#38500'(&D)'
      OnExecute = DelFromHistActionExecute
      OnUpdate = DelFromHistActionUpdate
    end
    object ClrFindHistAction: TAction
      Caption = #26908#32034#23653#27508#12434#12377#12409#12390#21066#38500'(&A)'
      OnExecute = ClrFindHistActionExecute
      OnUpdate = ClrFindHistActionUpdate
    end
    object StartAction: TAction
      Caption = #38283#22987
      OnExecute = StartActionExecute
      OnUpdate = StartActionUpdate
    end
  end
end
