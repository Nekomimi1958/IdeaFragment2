object IdeaFragMainForm: TIdeaFragMainForm
  Left = 0
  Top = 0
  Cursor = crHandPoint
  HelpContext = 1
  Caption = 'IdeaFragment2'
  ClientHeight = 551
  ClientWidth = 792
  Color = clBtnFace
  ParentFont = True
  FormStyle = fsMDIForm
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 32
    Width = 4
    Height = 498
    Beveled = True
    Color = clAppWorkSpace
    ParentColor = False
    ResizeStyle = rsUpdate
    Visible = False
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 530
    Width = 792
    Height = 21
    Action = DispStatus
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Pitch = fpVariable
    Font.Style = []
    Panels = <
      item
        Alignment = taRightJustify
        Width = 50
      end
      item
        Width = 50
      end
      item
        Width = 50
      end
      item
        Width = 50
      end>
    UseSystemFont = False
  end
  object DockPanel1: TPanel
    Left = 4
    Top = 32
    Width = 0
    Height = 498
    Align = alLeft
    DockSite = True
    TabOrder = 1
    OnDockDrop = DockPanelDockDrop
    OnDockOver = DockPanelDockOver
    OnGetSiteInfo = DockPanelGetSiteInfo
    OnUnDock = DockPanelUnDock
  end
  object DockPanel2: TPanel
    Left = 0
    Top = 32
    Width = 792
    Height = 0
    Align = alTop
    DockSite = True
    TabOrder = 2
    OnDockDrop = DockPanelDockDrop
    OnDockOver = DockPanelDockOver
    OnGetSiteInfo = DockPanelGetSiteInfo
    OnUnDock = DockPanelUnDock
  end
  object DockPanel3: TPanel
    Left = 792
    Top = 32
    Width = 0
    Height = 498
    Align = alRight
    DockSite = True
    TabOrder = 3
    OnDockDrop = DockPanelDockDrop
    OnDockOver = DockPanelDockOver
    OnGetSiteInfo = DockPanelGetSiteInfo
    OnUnDock = DockPanelUnDock
  end
  object DockPanel4: TPanel
    Left = 0
    Top = 530
    Width = 792
    Height = 0
    Align = alBottom
    DockSite = True
    TabOrder = 4
    OnDockDrop = DockPanelDockDrop
    OnDockOver = DockPanelDockOver
    OnGetSiteInfo = DockPanelGetSiteInfo
    OnUnDock = DockPanelUnDock
  end
  object GroupListBox: TListBox
    Left = 8
    Top = 48
    Width = 100
    Height = 73
    HelpContext = 33
    BorderStyle = bsNone
    DragKind = dkDock
    DragMode = dmAutomatic
    ImeMode = imClose
    ItemHeight = 13
    PopupMenu = GlistPopupMenu
    TabOrder = 5
    Visible = False
    OnClick = GroupListBoxClick
    OnDblClick = GroupListBoxDblClick
    OnDragDrop = GroupListBoxDragDrop
    OnDragOver = GroupListBoxDragOver
    OnDrawItem = GroupListBoxDrawItem
    OnEndDock = GroupListBoxEndDock
    OnEndDrag = GroupListBoxEndDrag
    OnEnter = GroupListBoxEnter
    OnExit = GroupListBoxExit
    OnKeyDown = GroupListBoxKeyDown
    OnMeasureItem = GroupListBoxMeasureItem
    OnMouseDown = GroupListBoxMouseDown
    OnMouseMove = GroupListBoxMouseMove
    OnStartDock = GroupListBoxStartDock
  end
  object ToolPanel: TControlBar
    Left = 0
    Top = 0
    Width = 792
    Height = 32
    HelpContext = 20
    Align = alTop
    AutoSize = True
    BevelEdges = [beTop]
    Color = clBtnFace
    GradientEndColor = 10921638
    ParentBackground = False
    ParentColor = False
    ParentShowHint = False
    RowSize = 32
    ShowHint = True
    TabOrder = 6
    OnDockOver = ToolPanelDockOver
    DesignSize = (
      792
      30)
    object ToolBar1: TToolBar
      Left = 11
      Top = 2
      Width = 201
      Height = 26
      Align = alNone
      Anchors = []
      AutoSize = True
      ButtonHeight = 26
      ButtonWidth = 27
      Caption = #27161#28310
      DragKind = dkDock
      DragMode = dmAutomatic
      GradientEndColor = 10921638
      Images = BtnImgList
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      Visible = False
      OnStartDock = ToolBarsStartDock
      object NewBtn: TToolButton
        Left = 0
        Top = 0
        Action = FileNew
      end
      object OpenBtn: TToolButton
        Left = 27
        Top = 0
        Action = FileOpen
      end
      object SaveBtn: TToolButton
        Left = 54
        Top = 0
        Action = FileSave
      end
      object ToolButton2: TToolButton
        Left = 81
        Top = 0
        Width = 6
        Caption = 'ToolButton2'
        ImageIndex = 2
        Style = tbsSeparator
      end
      object PrintBtn: TToolButton
        Left = 87
        Top = 0
        Action = FilePrint
      end
      object ToolButton4: TToolButton
        Left = 114
        Top = 0
        Width = 6
        Caption = 'ToolButton4'
        ImageIndex = 3
        Style = tbsSeparator
      end
      object CutBtn: TToolButton
        Left = 120
        Top = 0
        Action = EditCut
      end
      object CopyBtn: TToolButton
        Left = 147
        Top = 0
        Action = EditCopy
      end
      object PasteBtn: TToolButton
        Left = 174
        Top = 0
        Action = EditPaste
      end
    end
    object ToolBar2: TToolBar
      Left = 225
      Top = 2
      Width = 402
      Height = 26
      Align = alNone
      Anchors = []
      AutoSize = True
      ButtonHeight = 26
      ButtonWidth = 27
      Caption = #26029#29255#25805#20316
      DragKind = dkDock
      GradientEndColor = 10921638
      Images = BtnImgList
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      Visible = False
      OnStartDock = ToolBarsStartDock
      object EditNewBtn: TToolButton
        Left = 0
        Top = 0
        Action = EditNew
      end
      object ToolButton1: TToolButton
        Left = 27
        Top = 0
        Width = 6
        Caption = 'ToolButton1'
        ImageIndex = 7
        Style = tbsSeparator
      end
      object EditGatherBtn: TToolButton
        Left = 33
        Top = 0
        Action = EditGather
      end
      object EditSpreadBtn: TToolButton
        Left = 60
        Top = 0
        Action = EditSpread
      end
      object EditStretchBtn: TToolButton
        Left = 87
        Top = 0
        Action = EditStretch
      end
      object EditGStretchBtn: TToolButton
        Left = 114
        Top = 0
        Action = EditGrpStretch
      end
      object EditAwayBtn: TToolButton
        Left = 141
        Top = 0
        Action = EditAway
      end
      object ToolButton10: TToolButton
        Left = 168
        Top = 0
        Width = 6
        Caption = 'ToolButton10'
        ImageIndex = 13
        Style = tbsSeparator
      end
      object EditUndoPosBtn: TToolButton
        Left = 174
        Top = 0
        Action = EditUndo
      end
      object EditRedoPosBtn: TToolButton
        Left = 201
        Top = 0
        Action = EditRedo
      end
      object ToolButton3: TToolButton
        Left = 228
        Top = 0
        Width = 6
        Caption = 'ToolButton3'
        ImageIndex = 13
        Style = tbsSeparator
      end
      object EditFindBtn: TToolButton
        Left = 234
        Top = 0
        Action = EditFind
      end
      object EditPrevSelBtn: TToolButton
        Left = 261
        Top = 0
        Action = EditPrevSel
      end
      object EditNextSelBtn: TToolButton
        Left = 288
        Top = 0
        Action = EditNextSel
      end
      object ToolButton8: TToolButton
        Left = 315
        Top = 0
        Width = 6
        Caption = 'ToolButton8'
        ImageIndex = 16
        Style = tbsSeparator
      end
      object GoBackBtn: TToolButton
        Left = 321
        Top = 0
        Action = GoBack
      end
      object GoForwardBtn: TToolButton
        Left = 348
        Top = 0
        Action = GoForward
      end
      object GoHomeBtn: TToolButton
        Left = 375
        Top = 0
        Action = GoHome
      end
    end
    object ToolBar3: TToolBar
      Left = 640
      Top = 2
      Width = 141
      Height = 26
      Align = alNone
      Anchors = []
      AutoSize = True
      ButtonHeight = 26
      ButtonWidth = 27
      Caption = #12514#12540#12489#20999#26367
      DragKind = dkDock
      GradientEndColor = 10921638
      Images = BtnImgList
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      Visible = False
      OnStartDock = ToolBarsStartDock
      object EditNormalBtn: TToolButton
        Left = 0
        Top = 0
        Action = ToolNrm
        Grouped = True
        Style = tbsCheck
      end
      object EditGroupBtn: TToolButton
        Left = 27
        Top = 0
        Action = ToolGruop
        Grouped = True
        Style = tbsCheck
      end
      object EditGLineBtn: TToolButton
        Left = 54
        Top = 0
        Action = ToolGLine
        Grouped = True
        Style = tbsCheck
      end
      object EditBrushBtn: TToolButton
        Left = 81
        Top = 0
        Action = ToolBrush
        Grouped = True
        Style = tbsCheck
      end
      object ToolButton5: TToolButton
        Left = 108
        Top = 0
        Width = 6
        Caption = 'ToolButton5'
        ImageIndex = 22
        Style = tbsSeparator
      end
      object BrushPanel: TPanel
        Left = 114
        Top = 0
        Width = 27
        Height = 26
        Hint = #32972#26223#22615#12426#33394
        BevelInner = bvLowered
        Color = clWhite
        TabOrder = 0
        StyleElements = [seBorder]
        OnClick = BrushPanelClick
      end
    end
  end
  object MainMenu1: TMainMenu
    AutoHotkeys = maManual
    Images = BtnImgList
    Left = 208
    Top = 48
    object File1: TMenuItem
      Caption = #12501#12449#12452#12523'(&F)'
      HelpContext = 5
      OnClick = File1Click
      object FileNewItem: TMenuItem
        Action = FileNew
      end
      object FileOpenItem: TMenuItem
        Action = FileOpen
      end
      object FileHistoryItem: TMenuItem
        Caption = #26368#36817#38283#12356#12383#12501#12449#12452#12523'(&H)'
      end
      object FileMergeItem: TMenuItem
        Action = FileMerge
      end
      object FileSaveItem: TMenuItem
        Action = FileSave
      end
      object FileSaveAsItem: TMenuItem
        Action = FileSaveAs
      end
      object FileCloseItem: TMenuItem
        Action = FileClose
      end
      object FileAllSaveEndItem: TMenuItem
        Action = FileAllSaveEnd
      end
      object F_s1: TMenuItem
        Caption = '-'
      end
      object FileImportItem: TMenuItem
        Caption = #12452#12531#12509#12540#12488'(&I)'
        object TextImportItem: TMenuItem
          Action = TextImport
        end
      end
      object FileExportItem: TMenuItem
        Caption = #12456#12463#12473#12509#12540#12488'(&O)'
        object TextExportItem: TMenuItem
          Action = TextExport
        end
        object CsvExportItem: TMenuItem
          Action = CsvExport
        end
        object TABT1: TMenuItem
          Action = TsvExport
        end
      end
      object F_s2: TMenuItem
        Caption = '-'
      end
      object PrintItem: TMenuItem
        Action = FilePrint
      end
      object FileImgOutItem: TMenuItem
        Action = FileImgOut
      end
      object F_s3: TMenuItem
        Caption = '-'
      end
      object FileExitItem: TMenuItem
        Action = FileExit1
      end
    end
    object Edit1: TMenuItem
      Caption = #32232#38598'(&E)'
      HelpContext = 6
      OnClick = Edit1Click
      object EditUndoPosItem: TMenuItem
        Action = EditUndo
      end
      object EditRedoPosItem: TMenuItem
        Action = EditRedo
      end
      object E_s1: TMenuItem
        Caption = '-'
      end
      object CutItem: TMenuItem
        Action = EditCut
      end
      object CopyItem: TMenuItem
        Action = EditCopy
      end
      object PasteItem: TMenuItem
        Action = EditPaste
      end
      object PasteExItem: TMenuItem
        Action = EditPasteEx
      end
      object E_s2: TMenuItem
        Caption = '-'
      end
      object EditNewItem: TMenuItem
        Action = EditNew
      end
      object EditNewClipItem: TMenuItem
        Action = EditNewClip
      end
      object EditDeleteItem: TMenuItem
        Action = EditDelete
      end
      object E_s3: TMenuItem
        Caption = '-'
      end
      object EditSelAllItem: TMenuItem
        Action = EditSelAll
      end
      object EditSelGroupItem: TMenuItem
        Caption = #12464#12523#12540#12503#36984#25246'(&G)'
        Hint = #25351#23450#12375#12383#12464#12523#12540#12503#12398#26029#29255#12434#36984#25246#12375#12414#12377
        OnClick = EditSelGroupItemClick
      end
      object EditSelBgColItem: TMenuItem
        Action = EditSelBgCol
      end
      object E_s4: TMenuItem
        Caption = '-'
      end
      object EditGatherItem: TMenuItem
        Action = EditGather
      end
      object EditSpreadItem: TMenuItem
        Action = EditSpread
      end
      object EditStretchItem: TMenuItem
        Action = EditStretch
      end
      object EditGrpStretchItem: TMenuItem
        Action = EditGrpStretch
      end
      object EditAwayItem: TMenuItem
        Action = EditAway
      end
      object EditMovePosItem: TMenuItem
        Action = EditMovePos
      end
      object EditTrimItem: TMenuItem
        Action = EditTrim
      end
      object E_s5: TMenuItem
        Caption = '-'
      end
      object EditMoveToGroupItem: TMenuItem
        Caption = #20182#12464#12523#12540#12503#12408#31227#21205'(&M)'
        Hint = #36984#25246#20013#12398#26029#29255#12434#20182#12398#12464#12523#12540#12503#12408#31227#21205#12375#12414#12377
        OnClick = EditMoveToGroupItemClick
      end
      object EditMoveToWinItem: TMenuItem
        Caption = #20182#12454#12451#12531#12489#12454#12408#31227#21205'(&V)'
        Hint = #36984#25246#20013#12398#26029#29255#12434#20182#12398#12454#12451#12531#12489#12454#12408#31227#21205#12375#12414#12377
        OnClick = EditMoveToWinItemClick
      end
      object E_s6: TMenuItem
        Caption = '-'
      end
      object EditFindItem: TMenuItem
        Action = EditFind
      end
      object EditPrevSelItem: TMenuItem
        Action = EditPrevSel
      end
      object EditNextSelItem: TMenuItem
        Action = EditNextSel
      end
      object E_s7: TMenuItem
        Caption = '-'
      end
      object EditPropertyItem: TMenuItem
        Action = EditProperty
      end
    end
    object Go1: TMenuItem
      Caption = #12472#12515#12531#12503'(&G)'
      HelpContext = 7
      OnClick = Go1Click
      object GoJumpItem: TMenuItem
        Action = GoJump
      end
      object GoBackItem: TMenuItem
        Action = GoBack
      end
      object GoForwardItem: TMenuItem
        Action = GoForward
      end
      object GoHomeItem: TMenuItem
        Action = GoHome
      end
      object G_s1: TMenuItem
        Caption = '-'
      end
      object GoListItem: TMenuItem
        Caption = #23653#27508'(&L)'
      end
    end
    object Tool1: TMenuItem
      Caption = #12484#12540#12523'(&T)'
      HelpContext = 8
      OnClick = Tool1Click
      object ToolNrmItem: TMenuItem
        Action = ToolNrm
      end
      object ToolGroupItem: TMenuItem
        Action = ToolGruop
      end
      object ToolGLineItem: TMenuItem
        Action = ToolGLine
      end
      object ToolBrushItem: TMenuItem
        Action = ToolBrush
      end
      object T_s1: TMenuItem
        Caption = '-'
      end
      object ToolFindExItem: TMenuItem
        Action = ToolFindEx
      end
      object ToolRubberLayItem: TMenuItem
        Action = RubberLay
      end
    end
    object Show1: TMenuItem
      Caption = #34920#31034'(&V)'
      HelpContext = 9
      OnClick = Show1Click
      object ShowGrpLstItem: TMenuItem
        Action = ShowGrpLst
        HelpContext = 17
      end
      object ShowFrgGridItem: TMenuItem
        Action = ShowFrgGrid
        HelpContext = 25
      end
      object ShowMapViewerItem: TMenuItem
        Action = ShowMapViewer
        HelpContext = 18
      end
      object V_s1: TMenuItem
        Caption = '-'
      end
      object ShowNamePlateItem: TMenuItem
        Action = ShowNamePlate
      end
      object ShowLinkMarkItem: TMenuItem
        Action = ShowLinkMark
      end
      object ShowGFrameItem: TMenuItem
        Action = ShowGFrame
      end
      object ShowGLineItem: TMenuItem
        Action = ShowGLine
      end
      object ShowFLineItem: TMenuItem
        Action = ShowFLine
      end
      object ShowDivLineItem: TMenuItem
        Action = ShowDivLine
      end
      object ShowPosGridItem: TMenuItem
        Action = ShowPosGrid
      end
      object V_s2: TMenuItem
        Caption = '-'
      end
      object ShowToolBarItem: TMenuItem
        Caption = #12484#12540#12523#12496#12540'(&T)'
        HelpContext = 9
        object ShowTool1Item: TMenuItem
          Caption = #27161#28310'(&S)'
          HelpContext = 9
          OnClick = ShowToolItemClick
        end
        object ShowTool2Item: TMenuItem
          Tag = 1
          Caption = #26029#29255#25805#20316'(&F)'
          HelpContext = 9
          OnClick = ShowToolItemClick
        end
        object ShowTool3Item: TMenuItem
          Tag = 2
          Caption = #12514#12540#12489#20999#12426#26367#12360'(&M)'
          HelpContext = 9
          OnClick = ShowToolItemClick
        end
      end
      object ShowStatusBarItem: TMenuItem
        Action = ShowStatusBar
      end
    end
    object Window1: TMenuItem
      Caption = #12454#12451#12531#12489#12454'(&W)'
      HelpContext = 10
      OnClick = Window1Click
      object WindowTileHItem: TMenuItem
        Action = WindowTileH
      end
      object WindowTileVItem: TMenuItem
        Action = WindowTileV
      end
      object WindowCascadeItem: TMenuItem
        Action = WindowCascade
      end
      object WindowArrangeItem: TMenuItem
        Action = WindowArrange
      end
      object WindowActMaxItem: TMenuItem
        Action = WindowActMax
      end
      object W_s1: TMenuItem
        Caption = '-'
      end
    end
    object Option1: TMenuItem
      Caption = #12458#12503#12471#12519#12531'(&O)'
      object OptionSetupItem: TMenuItem
        Action = OptionSetup
      end
      object O_s1: TMenuItem
        Caption = '-'
      end
      object OptionScrSizeItem: TMenuItem
        Action = OptionIndividual
      end
    end
    object Help1: TMenuItem
      Caption = #12504#12523#12503'(&H)'
      HelpContext = 11
      object HelpContentsItem: TMenuItem
        Action = HelpContents
      end
      object H_s1: TMenuItem
        Caption = '-'
      end
      object HelpHomePageItem: TMenuItem
        Action = HelpHomePage
      end
      object H_s2: TMenuItem
        Caption = '-'
      end
      object HelpAboutItem: TMenuItem
        Action = HelpAbout
      end
    end
  end
  object OpenDialog: TOpenDialog
    Filter = #12377#12409#12390#12398#65420#65383#65394#65433' (*.*)|*.*'
    Left = 571
    Top = 102
  end
  object SaveDialog: TSaveDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 639
    Top = 102
  end
  object EditPopupMenu: TPopupMenu
    AutoHotkeys = maManual
    Images = BtnImgList
    OnPopup = EditPopupMenuPopup
    Left = 208
    Top = 102
    object PopUndoPosItem: TMenuItem
      Action = EditUndo
    end
    object PopRedoPosItem: TMenuItem
      Action = EditRedo
    end
    object P_s1: TMenuItem
      Caption = '-'
    end
    object PopCutItem: TMenuItem
      Action = EditCut
    end
    object PopCopyItem: TMenuItem
      Action = EditCopy
    end
    object PopPasteItem: TMenuItem
      Action = EditPaste
    end
    object PopPasteExItem: TMenuItem
      Action = EditPasteEx
    end
    object P_s2: TMenuItem
      Caption = '-'
    end
    object PopNewItem: TMenuItem
      Action = EditNew
    end
    object PopNewClipItem: TMenuItem
      Action = EditNewClip
    end
    object PopDeleteItem: TMenuItem
      Action = EditDelete
    end
    object P_s3: TMenuItem
      Caption = '-'
    end
    object PopSelAllItem: TMenuItem
      Action = EditSelAll
    end
    object PopSelGroupItem: TMenuItem
      Caption = #12464#12523#12540#12503#36984#25246'(&G)'
      HelpContext = 5
      OnClick = EditSelGroupItemClick
    end
    object PopGatherItem: TMenuItem
      Action = EditGather
    end
    object PopSpreadItem: TMenuItem
      Action = EditSpread
    end
    object PopStretchItem: TMenuItem
      Action = EditStretch
    end
    object PopGrpStretchItem: TMenuItem
      Action = EditGrpStretch
    end
    object PopAwayItem: TMenuItem
      Action = EditAway
    end
    object PopMovePosItem: TMenuItem
      Action = EditMovePos
    end
    object P_s4: TMenuItem
      Caption = '-'
    end
    object PopMoveToGroupItem: TMenuItem
      Caption = #20182#12464#12523#12540#12503#12408#31227#21205'(&M)'
      HelpContext = 5
      OnClick = EditMoveToGroupItemClick
    end
    object PopMoveToWinItem: TMenuItem
      Caption = #20182#12454#12451#12531#12489#12454#12408#31227#21205'(&V)'
      HelpContext = 5
      OnClick = EditMoveToWinItemClick
    end
    object P_s5: TMenuItem
      Caption = '-'
    end
    object PopFindItem: TMenuItem
      Action = EditFind
    end
    object PopJumpItem: TMenuItem
      Action = GoJump
    end
    object IDI2: TMenuItem
      Action = ExCopyAnchor
    end
    object P_s6: TMenuItem
      Caption = '-'
    end
    object PopPropertyItem: TMenuItem
      Action = EditProperty
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 250
    OnTimer = Timer1Timer
    Left = 120
    Top = 97
  end
  object GlistPopupMenu: TPopupMenu
    AutoHotkeys = maManual
    HelpContext = 33
    OnPopup = GlistPopupMenuPopup
    Left = 310
    Top = 46
    object GListUpItem: TMenuItem
      Caption = #19978#12408'(&U)'
      OnClick = GListUpItemClick
    end
    object GListDownItem: TMenuItem
      Caption = #19979#12408'(&D)'
      OnClick = GListDownItemClick
    end
    object Gl_s1: TMenuItem
      Caption = '-'
    end
    object GListShowAllItem: TMenuItem
      Caption = #20840#12390#12398#12464#12523#12540#12503#12434#34920#31034'(&A)'
      OnClick = GListDispItemClick
    end
    object GListNotBlankItem: TMenuItem
      Tag = 1
      Caption = #31354#12391#12394#12356#12464#12523#12540#12503#12434#34920#31034'(&B)'
      OnClick = GListDispItemClick
    end
    object GListClrAllItem: TMenuItem
      Tag = 2
      Caption = #20840#12390#12398#12464#12523#12540#12503#12434#38560#12377'(&C)'
      OnClick = GListDispItemClick
    end
    object Gl_s2: TMenuItem
      Caption = '-'
    end
    object GListHideBlkItem: TMenuItem
      Caption = #31354#12464#12523#12540#12503#38917#30446#12399#38750#34920#31034'(&L)'
      OnClick = GListHideBlkItemClick
    end
    object Gl_s3: TMenuItem
      Caption = '-'
    end
    object GListPosItem: TMenuItem
      Caption = #12522#12473#12488#12398#20301#32622'(&P)'
      object GListLeftItem: TMenuItem
        Tag = 1
        Caption = #24038'(&L)'
        OnClick = GListPosItemClick
      end
      object GListRightItem: TMenuItem
        Tag = 3
        Caption = #21491'(&R)'
        OnClick = GListPosItemClick
      end
      object GListTopItem: TMenuItem
        Tag = 2
        Caption = #19978'(&T)'
        OnClick = GListPosItemClick
      end
      object GListBottomItem: TMenuItem
        Tag = 4
        Caption = #19979'(&B)'
        OnClick = GListPosItemClick
      end
      object Gl_s4: TMenuItem
        Caption = '-'
      end
      object GListDockFlist: TMenuItem
        Tag = 10
        Caption = #26029#29255#12522#12473#12488'(&D)'
        OnClick = GListPosItemClick
      end
      object Gl_s5: TMenuItem
        Caption = '-'
      end
      object GListFloatItem: TMenuItem
        Caption = #28014#21205'(&F)'
        OnClick = GListFloatItemClick
      end
    end
    object GListHideItem: TMenuItem
      Caption = #12522#12473#12488#12434#38560#12377'(&H)'
      OnClick = GListHideItemClick
    end
  end
  object PrintDialog1: TPrintDialog
    Left = 571
    Top = 158
  end
  object GLinePopupMenu: TPopupMenu
    AutoHotkeys = maManual
    Images = BtnImgList
    OnPopup = GLinePopupMenuPopup
    Left = 308
    Top = 102
    object GLineType0Item: TMenuItem
      Caption = #12479#12452#12503' : '#32218' (&L)'
      OnClick = GLineTypeItemClick
    end
    object GLineType1Item: TMenuItem
      Tag = 1
      Caption = #12479#12452#12503' : '#30690#21360'(&A)'
      OnClick = GLineTypeItemClick
    end
    object GLineType5Item: TMenuItem
      Tag = 5
      Caption = #12479#12452#12503' : '#36870#30690#21360'(&P)'
      OnClick = GLineTypeItemClick
    end
    object GLineType2Item: TMenuItem
      Tag = 2
      Caption = #12479#12452#12503' : '#20001#30690#21360'(&B)'
      OnClick = GLineTypeItemClick
    end
    object GLineType3Item: TMenuItem
      Tag = 3
      Caption = #12479#12452#12503' : '#36870#20001#30690#21360'(&C)'
      OnClick = GLineTypeItemClick
    end
    object GLineType4Item: TMenuItem
      Tag = 4
      Caption = #12479#12452#12503' : '#20108#37325#32218'(&E)'
      OnClick = GLineTypeItemClick
    end
    object GLine_s1: TMenuItem
      Caption = '-'
    end
    object GLineDotItem: TMenuItem
      Tag = -10
      Caption = #28857#32218' (&T)'
      OnClick = GLineTypeItemClick
    end
    object GLineWd0Item: TMenuItem
      Tag = 10
      Caption = #32048#32218' (&1)'
      OnClick = GLineTypeItemClick
    end
    object GLineWd1Item: TMenuItem
      Tag = 11
      Caption = #20013#22826#32218' (&3)'
      OnClick = GLineTypeItemClick
    end
    object GLineWd2Item: TMenuItem
      Tag = 12
      Caption = #22826#32218' (&5)'
      OnClick = GLineTypeItemClick
    end
    object GLineWd3Item: TMenuItem
      Tag = 13
      Caption = #26997#22826#32218'(9)'
      OnClick = GLineTypeItemClick
    end
    object GLine_s2: TMenuItem
      Caption = '-'
    end
    object GLineAtItem: TMenuItem
      Caption = #30690#12398#24418#29366'(&S)'
      object GLineAtSldItem: TMenuItem
        Tag = 20
        Caption = #9660
        OnClick = GLineTypeItemClick
      end
      object GLineAtLinItem: TMenuItem
        Tag = 21
        Caption = 'V'
        OnClick = GLineTypeItemClick
      end
    end
    object GLineAsItem: TMenuItem
      Caption = #30690#12398#12469#12452#12474'(&Z)'
      object GLineAsSItem: TMenuItem
        Tag = 30
        Caption = #23567
        OnClick = GLineTypeItemClick
      end
      object GLineAsMItem: TMenuItem
        Tag = 31
        Caption = #20013
        OnClick = GLineTypeItemClick
      end
      object GLineAsLItem: TMenuItem
        Tag = 32
        Caption = #22823' '
        OnClick = GLineTypeItemClick
      end
    end
    object GLine_s3: TMenuItem
      Caption = '-'
    end
    object GLineLblItem: TMenuItem
      Caption = #12521#12505#12523#20837#21147'(&I)'
      OnClick = GLineLblItemClick
    end
    object GLineOpsItem: TMenuItem
      Caption = #12521#12505#12523#12434#21453#23550#20596#12395'(&O)'
      OnClick = GLineOpsItemClick
    end
    object GLineColItem: TMenuItem
      Caption = #32218#12398#20491#21029#33394#25351#23450'(&J)...'
      OnClick = GLineColItemClick
    end
    object GLine_s4: TMenuItem
      Caption = '-'
    end
    object GLineJncItem: TMenuItem
      Caption = #20998#21106#28857#12434#25407#20837'(&V)'
      OnClick = GLineJncItemClick
    end
    object GLineRevItem: TMenuItem
      Caption = #26041#21521#21453#36578'(&R)'
      OnClick = GLineRevItemClick
    end
    object GLineDelItem: TMenuItem
      Caption = #38306#20418#32218#12398#21066#38500'(&D)'
      OnClick = GLineDelItemClick
    end
    object GLine_s5: TMenuItem
      Caption = '-'
    end
    object GLineRetItem: TMenuItem
      Action = ToolNrm
    end
  end
  object BtnImgList: TImageList
    Height = 20
    Width = 20
    Left = 499
    Top = 102
    Bitmap = {
      494C010121002500680014001400FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      000000000000360000002800000050000000B4000000010020000000000000E1
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000FF000000FF000000FF000000FF0000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484008484840084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000000000000000
      0000000000000000000000000000840000008400000084000000840000008400
      0000840000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      000000000000FF000000FF000000FF000000FF000000FF000000000000000000
      00000000000000000000000000000000000000000000FFFF0000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00840000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00840000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      00000000000000000000000000000000000000000000FFFF0000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00840000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      84008484840000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      00000000000000000000000000000000000000000000FFFF0000000000000000
      0000000000000000000000000000840000008400000084000000840000008400
      0000840000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      84008484840000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      00000000000000000000000000000000000000000000FFFF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      00000000000000000000000000000000000000000000FFFF0000000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000000000000FFFF00000000
      000000000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF00000000000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000848484008484
      84008484840000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      00000000000000000000000000000000000000000000FFFF0000000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF008400000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008484
      84008484840000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      00000000000000000000000000000000000000000000FFFF0000000000008400
      0000840000008400000084000000840000008400000000000000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF008400000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF008400000000000000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF008400000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF008400000000000000000000008400
      0000840000008400000084000000840000008400000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF008400000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      84008484840084848400000000000000000000000000FFFF0000000000008400
      0000840000008400000084000000840000008400000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400848484008484840000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484008484840084848400848484008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      840084848400848484000000000000000000000000000000000000000000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000000000000000000000000000000000
      0000000000000000000000000000840000008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000084000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0084000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008400840084008400848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00840000000000000000000000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008400
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000084000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0084000000000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      840084008400FFFFFF00FFFFFF00C6C6C6008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00840000000000000000000000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008400
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000084000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0084000000000000000000
      000000000000000000000000000000000000000000008400840084008400FFFF
      FF00FFFFFF000000000000000000C6C6C600C6C6C60084848400000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00840000000000000000000000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008400
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000000000000000
      00000000000000000000000000008400840084008400FFFFFF00FFFFFF000000
      000000000000840084008400840000000000C6C6C600C6C6C600848484000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000000000000000000000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008400
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000084000000FFFFFF00FFFFFF008400000084000000840000008400
      000084000000840000008400000084000000FFFFFF0084000000000000000000
      000000000000000000000000000084008400FFFFFF0000000000000000008400
      84008400840084008400840084008400840000000000C6C6C600C6C6C6008484
      8400000000000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      00008400000084000000FFFFFF00840000000000000000000000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008400
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000084000000FFFFFF00FFFFFF008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000000000000000
      0000000000000000000000000000000000000000000084008400840084008400
      84000000000000FFFF0084008400840084008400840000000000C6C6C600C6C6
      C600848484000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000000000000000000000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008400
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0084000000000000000000000000000000000000000000
      0000000000000000000000000000840084008400840084008400840084008400
      840084008400000000008400840084008400840084008400840000000000C6C6
      C600000000000000000000000000000000000000000000000000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00840000000000000000000000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008400
      0000000000000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000000000000000000000000000000000000000
      000000000000000000000000000084008400FFFFFF0084008400840084008400
      8400840084008400840000FFFF0000FFFF008400840084008400840084000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00840000000000000000000000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008400
      0000000000000000000000000000000000000000000000000000000000008400
      0000FFFFFF008400000084000000840000008400000084000000840000008400
      000084000000FFFFFF0084000000000000000000000000000000000000000000
      00000000000000000000000000000000000084008400FFFFFF00840084008400
      84008400840084008400840084000000000000FFFF0000FFFF00840084008400
      8400000000000000000000000000000000000000000000000000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00840000000000000000000000000000000000
      000000000000000000000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008400
      0000000000000000000000000000000000000000000000000000000000008400
      0000FFFFFF008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084008400FFFFFF008400
      84008400840084008400000000008400840000FFFF0000FFFF00840084008400
      8400840084000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000000000000000000000000000000000
      0000000000000000000000000000840000008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000000000000000000000000000000000000000000000000000000000008400
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00840000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000084008400FFFF
      FF00840084008400840000FFFF0000FFFF0000FFFF0084008400840084008400
      8400000000000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      00008400000084000000FFFFFF00840000000000000000000000000000000000
      0000000000000000000000000000840000008400000084000000840000008400
      0000FFFFFF008400000084000000840000008400000084000000FFFFFF008400
      0000000000000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      8400FFFFFF008400840084008400840084008400840084008400000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000000000000000000000000000000000
      0000000000000000000000000000840000008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000000000000000000000000000000000000000000000000000000000008400
      000084000000840000008400000084000000840000008400000084000000FFFF
      FF00840000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000084008400FFFFFF0084008400840084000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008400840084008400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007B7B7B0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF007B7B7B0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008400000084000000840000008400
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF007B7B7B0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007B7B7B00000000007B7B7B000000
      0000000000000000000000000000000000008400000084000000840000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF007B7B7B0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000008400000084000000000000C6C6C600C6C6C600C6C6C600C6C6
      C600000000000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000007B7B7B0000000000FFFFFF00000000000000
      0000000000000000000000000000000000008400000084000000840000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF007B7B7B0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000840000008400000084000000000000C6C6C600C6C6C6000000
      0000008400000084000000840000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007B7B7B000000
      000000000000000000007B7B7B0000000000FFFFFF00000000007B7B7B000000
      0000000000000000000000000000000000008400000000000000000000008400
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000000000000000000000FFFF0000FFFF007B7B7B0000000000000000000000
      0000000000000000000000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000000000000000000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007B7B7B0000000000FFFFFF00000000007B7B7B00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000840000000000000000000000000000000000000000000000000000000000
      0000FFFF00000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF007B7B7B0000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000840000008400000084000000000000C6C6C600C6C6C6000000
      0000008400000084000000840000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF000000000000000000FFFFFF00000000007B7B7B0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000840000000000000000000000000000000000000000000000FFFF
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF007B7B7B0000000000000000000000
      000000000000FFFFFF007B7B7B00FFFFFF0000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000008400000084000000000000C6C6C600C6C6C600C6C6C600C6C6
      C600000000000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00000000007B7B7B000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000008400000000000000000000008400000000000000FFFF
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF007B7B7B0000000000000000000000
      0000FFFFFF007B7B7B00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000084000000840000008400000000000000FFFF
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF007B7B7B000000000000000000FFFF
      FF007B7B7B00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000084000000840000008400000000000000FFFF
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF007B7B7B0000000000FFFFFF007B7B
      7B00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      000000000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000007B7B7B0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000008400000084000000840000008400000000000000FFFF
      0000000000008400000084000000840000008400000084000000840000000000
      000000000000000000000000000000FFFF0000000000FFFFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF00000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      00000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00840000000000
      000000000000000000000000000000FFFF0000FFFF0000000000FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FF000000FF000000FF000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      00000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00840000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF000000FF000000FF000000FF00
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      00000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00840000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      00000000000084000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00840000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      0000000000008400000084000000840000008400000084000000840000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848400008484
      0000848400008484000084840000848400008484000084840000848400008484
      0000848400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848400008484
      0000848400008484000084840000848400008484000084840000848400008484
      0000848400000000000000000000000000000000000000000000000000000000
      0000000000000000000084840000848400008484000084840000848400008484
      0000848400008484000084840000848400008484000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848400008484
      0000848400008484000084840000848400008484000084840000848400008484
      0000848400000000000000000000000000000000000000000000000000000000
      0000000000000000000084840000848400008484000084840000848400008484
      0000848400008484000084840000848400008484000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848400008484
      0000848400008484000084840000848400008484000084840000848400008484
      0000848400000000000000000000000000000000000000000000000000000000
      0000000000000000000084840000848400008484000084840000848400008484
      0000848400008484000084840000848400008484000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000008400000084000000840000008400000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000084
      0000008400000084000000840000008400000084000000000000000000000000
      0000000000000000000000000000000000000000000000000000848400008484
      0000848400008484000084840000848400008484000084840000848400008484
      0000848400000000000000000000000000000000000000000000000000000000
      0000000000000000000084840000848400008484000084840000848400008484
      0000848400008484000084840000848400008484000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000084
      0000008400000084000000840000008400000084000000840000008400000000
      0000000000000000000000000000000000000000000000000000848400008484
      0000848400008484000084840000848400008484000084840000848400008484
      0000848400000000000000000000000000000000000000000000000000000000
      0000000000000000000084840000848400008484000084840000848400008484
      0000848400008484000084840000848400008484000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000008400000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000084
      0000008400000084000000840000008400000084000000840000000000000000
      0000000000000000000000000000000000000000000000000000848400008484
      0000848400008484000084840000848400008484000084840000848400008484
      0000848400000000000000000000000000000000000000000000000000000000
      0000000000000000000084840000848400008484000084840000848400008484
      0000848400008484000084840000848400008484000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000000840000008400000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000084
      0000008400000084000000840000008400000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848400008484
      0000848400008484000084840000848400008484000084840000848400008484
      0000848400000000000000000000000000000000000000000000000000000000
      0000000000000000000084840000848400008484000084840000848400008484
      0000848400008484000084840000848400008484000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848400008484
      0000848400008484000084840000848400008484000084840000848400008484
      0000848400000000000000000000000000000000000000000000000000000000
      0000000000000000000084840000848400008484000084840000848400008484
      0000848400008484000084840000848400008484000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084840000848400008484000084840000848400008484
      0000848400008484000084840000848400008484000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840084848400848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000084848400848484008484840084848400848484008484
      8400848484008484840084848400848484000000000000000000848484008484
      8400848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000FFFFFF000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000FFFFFF000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000FFFFFF000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000848484000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF000000000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00008400
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00008400
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00008400
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF00FFFF0000FFFF0000FFFF0000FFFF0000FFFF00008400
      0000FFFF00000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFF0000FFFF0000FFFF0000FFFF0000FFFF00008400
      0000FFFF00000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00840000008400000084000000840000008400
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FF00000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008400
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000000000000000
      0000000000000000000000000000000000000000000084000000000000008400
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000000000000000
      0000FF000000FF000000FF000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000084000000000000008400
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000000000000000
      000000000000FF00000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000084000000000000008400
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000084000000000000008400
      000000000000FFFFFF0000000000FFFFFF0084848400FFFFFF0000000000FFFF
      FF0084848400FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF0000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000FF000000000000000000000000000000FF00000000000000000000000000
      00000000000000000000FF00000000000000FFFF000000000000FF0000000000
      000000000000000000000000000000000000FF00000000000000FFFF00000000
      0000FF0000000000000000000000000000000000000084000000000000008400
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00000000000000000000000000FF000000FF00000000FF
      FF0000FFFF0000FFFF00FF000000FF000000000000000000000000000000FF00
      0000FF00000000FFFF0000FFFF0000FFFF00FF000000FF000000000000000000
      000000000000FF000000FF00000000FFFF00FFFF000000FFFF00FF000000FF00
      0000000000000000000000000000FF000000FF00000000FFFF00FFFF000000FF
      FF00FF000000FF00000000000000000000000000000084000000000000008400
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF0000000000FFFFFF0000000000FFFFFF00000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000FF0000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000FF000000000000000000000000000000FF00000000000000000000000000
      00000000000000000000FF00000000000000FFFF000000000000FF0000000000
      000000000000000000000000000000000000FF00000000000000FFFF00000000
      0000FF0000000000000000000000000000000000000084000000000000008400
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF0084848400FFFF
      FF0000000000FFFFFF0084848400FFFFFF0000000000FFFFFF00000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000084000000000000008400
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000084000000000000008400
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000000000000000
      000000000000FF00000000000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000084000000000000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000008400000084000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000000000000000
      0000FF000000FF000000FF000000000000000000000000000000FFFF00000000
      0000000000000000000000000000000000000000000084000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000000000000000
      0000000000000000000000000000000000000000000084000000840000008400
      0000840000008400000084000000840000008400000084000000840000008400
      0000840000008400000084000000840000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FF00000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400840000008400000084848400000000000000000000000000000000000000
      0000000000008484840084000000840000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008400
      0000000000000000000084000000848484000000000000000000000000000000
      0000848484008400000000000000000000008400000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF008484840084848400848484008484840084848400FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008400
      0000000000000000000000000000840000000000000000000000000000000000
      0000840000000000000000000000000000008400000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00848484008484840084848400848484008484840084848400FFFF
      FF00000000000000000000000000000000000000000000000000C6C6C600FFFF
      FF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848484008400
      0000000000000000000000000000840000000000000000000000000000000000
      0000840000000000000000000000000000008400000084848400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000FFFFFF00C6C6
      C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C60000000000FFFF
      FF00C6C6C600C6C6C600848484008484840084848400FFFFFF00000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000008484
      8400840000000000000000000000840000008484840000000000000000008484
      8400840000000000000000000000840000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00848484008484840084848400848484008484840084848400FFFF
      FF00000000000000000000000000000000000000000000000000C6C6C600FFFF
      FF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C60084000000840000008400
      00008400000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000848484008400000084000000840000008400000084848400848484008400
      0000840000008400000084000000848484000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000FFFFFF00C6C6
      C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FF0000008400
      000084000000C6C6C600848484008484840084848400FFFFFF00000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000084000000840000008400000084000000840000008400
      0000840000008400000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00848484008484840084848400848484008484840084848400FFFF
      FF00000000000000000000000000000000000000000000000000C6C6C600FFFF
      FF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C6008400000084000000FF00
      00008400000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000008400000084840000FFFFFF00FFFFFF008484
      0000840000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00C6C6C600C6C6C600C6C6C6000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000FFFFFF00C6C6
      C600FFFFFF00C6C6C600FFFFFF00C6C6C600840000008400000084000000FFFF
      FF0084000000FFFFFF00FFFFFF00FFFFFF008484840084848400000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF000000000000000000FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600FFFF
      FF00C6C6C600FFFFFF00C6C6C6008400000084000000840000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF008484840000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00C6C6C600C6C6C600C6C6C6000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00C6C6
      C600FFFFFF00C6C6C600840000008400000084000000C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF000000000000000000FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600FFFF
      FF00C6C6C600FFFFFF00C6C6C60084000000C6C6C600FFFFFF00C6C6C600FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00C6C6
      C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0084848400848484008484840000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C600FFFF
      FF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF00000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00C6C6
      C600840000008400000084000000840000008400000084000000FFFFFF00C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FF000000FF000000FF000000FF000000FF000000FF
      000000FF000000FF00000000000000000000000000000000000000000000FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFF0000FF000000FF000000FF000000FF00000084000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FF000000FF00008484
      840000000000000000000000000000000000000000000000000000000000FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFF0000FF000000FF000000FF000000FF00000084000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FF000000FF00008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FF000000FF00008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF0000FFFF000000000084848400848484008484840000FFFF0000FFFF008484
      840084848400848484008484840000FFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6000000000000000000000000000000000000000000000000000000
      000000FFFF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF000000000000000000000000000000
      0000000000000000000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF000000FF0000000000000084848400FF000000FF000000FFFFFF00C6C6
      C600C6C6C60000000000FF000000FF0000000000000000000000000000000000
      0000000000000000000000000000C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600C6C6C6000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000848484000000000000000000000000000000
      000000000000000000000000000000000000C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000FF000000FF0000000000000084848400FF000000FF000000FFFFFF00C6C6
      C600C6C6C60000000000FF000000FF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000848484000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      0000FF000000FF00000000000000C6C6C6008484840084848400C6C6C600C6C6
      C600C6C6C60000000000FF000000FF0000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00C6C6C600FFFFFF00C6C6C600FFFF
      FF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFF
      FF00C6C6C6000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000848484000000000000000000000000000000
      000000000000000000000000000000FFFF000000000000FFFF00C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C6000000000000000000000000000000000000000000000000000000
      0000FF000000FF00000084840000000000000000000000000000000000000000
      00000000000084840000FF000000FF0000000000000000000000000000000000
      0000000000000000000000000000C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6
      C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF000000
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000848484000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF000000000000FFFF00C6C6
      C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF000000000000000000000000000000000000000000000000000000
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF0000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00C6C6C600FFFFFF00C6C6C600FFFF
      FF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFF
      FF00C6C6C6000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000848484000000000000000000000000000000
      000000000000000000000000000000FFFF00FFFFFF0000000000C6C6C60000FF
      FF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FFFF00C6C6C60000FF
      FF00C6C6C60000FFFF0000000000000000000000000000000000000000000000
      0000FF0000008484000000000000000000000000000000000000000000000000
      0000000000000000000084840000FF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF0000FF
      FF0000FFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF000000000000FFFF0000FFFF0000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF00000000000000
      0000000000000000000000000000C6C6C60000FFFF00C6C6C60000FFFF00C6C6
      C60000FFFF00C6C6C60000000000000000000000000000000000000000000000
      0000FF00000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000FF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF0000FFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF000000000000FFFF0000FFFF0000FFFF00000000000000
      000000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF00000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000FF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF00000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000FF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF00000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000FF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      000000000000FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFF
      FF000000000000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000FFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF00000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000FF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      0000C6C6C600FFFFFF0000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484840000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF0000FFFF000000000000000000000000000000000000FFFF0000FFFF000000
      000000000000000000000000000000FFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000050000000B40000000100010000000000700800000000000000000000
      000000000000000000000000FFFFFF00FFFFF0000000000000000000FFFFF000
      0000000000000000FFF010000000000000000000FFF010000000000000000000
      FC1010000000000000000000FFF010000000000000000000F7F0100000000000
      00000000F7FFF0000000000000000000F7FEF0000000000000000000F7FC7000
      0000000000000000F7F830000000000000000000F7F010000000000000000000
      FFFEF0000000000000000000FFFEF000000000000000000080FEF00000000000
      0000000080FEF00000000000000000008080F000000000000000000080FFF000
      000000000000000080FFF0000000000000000000FFFFF0000000000000000000
      FFFFFFFFFFFFFFFFFFFF0000FFFFFFE03FFFBFFFFFFF0000E0007FE03FFFBFF8
      0FFF0000DFFFBFE03FFFBFF80FFF0000BE07DFE03FFFBFF8083F0000BE07DFE0
      3FFFBFF80FFF0000BE07DFFFFF83BC180FEF0000BE07DFFDFF839C1FFFEF0000
      BE07DFF8FF838C1F7FEF0000BFFFDFF07F82041E3FEF0000BFE0580001838C1C
      1FEF0000BFE05FFDFF839C180FEF0000A0605FFDFF83BC1F7FFF0000A0605FFF
      FFFFBFFF7FFF0000A0605FE03FFFBFFF7F010000A07FDFE03FFFBFFF7F010000
      A07FDFE03FFFBFFF01010000DFFFBFE03FFFBFFFFF010000E0007FE03FFFBFFF
      FF010000FFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFF0000FFFFFFFF
      FFFFFFFFFFFF0000FFFFFFFFFFFF003FF8FF0000E000FE000FFF003FE07F0000
      E000FE000FFF003F803F0000E000FE000FFF003E001F0000E000FE000FF8003E
      000F0000E000FE000FF8003E00070000E000FE000FF8003E08030000E000FE00
      0FF801FE04070000E000FE000FE001FE00070000E000FE000FE001FF01070000
      E000FE000FE001FF82030000E000FE000FE007FFC0030000E000FE000FE007FF
      E00F0000E000FE000FE007FFF03F0000FFFFFFFFFFE007FFF8FF0000FFFFFFFF
      FFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFF0000
      FFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFF7FFF0000FFFFFFFFFFFFFFFC
      7FFF0000F000FFFFFF0FFFFC7FFF0000F000FFFF1F1FFFFC7FFF0000F000FFFE
      1F1FFFFC7EFF0000F000FFDC1F6FF80C7C7F0000F000FFC83FF7F7FE783F0000
      F000FFC07FFBEFFE701F0000F000FF80FFFDAFFE600F0000F000FF81FFFE2FFE
      40070000E0007F00FFFE2FFE00030000F000FF007FFC281E00070000F801FE03
      FFFFE81E000F0000FC03FE0FFFFFE81FC01F0000FE07FC3FFFFFE81FE03F0000
      FF0FFCFFFFFFE81FF07F0000FF9FFFFFFFFFE81FF8FF0000FFFFFFFFFFFFEFFF
      FDFF0000FFFFFFFFFFFFF7FFFFFF0000FFFFFFFFFFFFFFFFFFFF0000FFFFFFFF
      F7FFFFFFFFFF0000FFFF7FFFE3FFFFFFFFFF0000FFFF7FFFC1FFFEFF7FFF0000
      80037FFFF7FFF8FF1FFF00008003780037FFE0FF07FF00008003780037FF80FF
      01FF00008003780037FE00FF007F00008003780037F800FF001F000080037800
      37E000FF000700008003780037F000FF000F00008003780037FC00FF003F0000
      8003780037FF00FF00FF00008003780037FF80FF01FF00008003780037FFE0FF
      07FF0000FFFF780037FFF8FF1FFF0000FFFC1FFFF7FFFEFF7FFF0000FFFE3FFF
      F7FFFFFFFFFF0000FFFF7FFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFF0000
      FFFFFFFFFFFFFFFFFFFF0000FFFFF8003FFC001FFF1F0000FFFFF8003FFC001F
      FF1F0000FFFF78003FFC001C00070000FFFEF8003FFC001800070000FFFDF800
      3FFC001800070000FFFBF8003FFC001800070000FFF7F80037EC001800070000
      FEEFFFFFF7EFFFF800070000FEDFFFFFFBDFFFF800070000FE3FFFFFFBDFFFF8
      00070000FE3FFFFFFBDFFFF800030000FE0FFFFFFBDFFFF800030000FFFFFF03
      FBDFC0F80003000080FFFF1FF7EFF8F80003000080FFFF1FF7EFF8F800030000
      80FFFF67CFF3E6FFF807000080FFFF781FF81EFFFC0F000080FFFF7FFFFFFEFF
      FE1F0000FFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFBFFFFBFF0000FFFFF81F
      FFFF1FFFF1FF0000F000181FC1FE0FFFE0FF0000E000181041FF1FFFF1FF0000
      C000181041FF1FFF803F00008000181041FE0FFF60DF00008000181041FF1FFF
      71DF000080001FFFFFDDB77D5B57000080001FE04180E0380E03000080001820
      4100401004010000800018204180E0380E0300008000182041DDF77D5F570000
      8000182041FFBFFF7BDF000080001FFFFFFF1FFF71DF0000800038103FFE0FFF
      60DF00008000781021FF1FFF803F00008000F81021FF1FFFF1FF0000FFFFF810
      21FE0FFFE0FF0000FFFFF81021FF1FFFF1FF0000FFFFFFFFFFFFBFFFFBFF0000
      FFFFFFFFFFFFFFFFFFFF0000FFFFFFE007FFC01FFFFF0000E1F87FE007FFC01F
      FFFF0000CCF33FE007C0001FFFFF0000CEF73FE00780001C00070000CEF73FE0
      0780001C00070000E6667C000780001C00070000F000FC000780001C00070000
      FC03FC000780001C00070000FE07FC000780001C00070000FE07FC000780003C
      00070000FC03FC000F80007C00070000F801FC001F8007FC00070000F060FC00
      FF8007FC00030000E0F07C00FF8007FC00030000C1F83C00FF8007FC00030000
      C3FC3C01FFC00FFFFF8F0000C7FE3C03FFF03FFFFF8F0000CFFF3FFFFFF87FFF
      FF8F0000FFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFF0000FFDFFFFF
      FFFFFFFFFFFF0000DFDFBFFFFFFFFFFE00070000E4007FFFFFE0007C00030000
      F000FC000FE0007C00030000F800FC0007E0007C00030000F800FC0007E0007C
      00030000F800FC0003E0007C00030000F800FC0003E0007C00030000F800FC00
      01E0007C00030000C0007C0001E0007F801F0000E0003C0003E0007F801F0000
      F801FC000FE0007F801F0000F803FC000FE0007F801F0000F803FC001FE0007F
      803F0000F809FE0FFFE0007F807F0000F01CFFFFFFE0007F80FF0000E79E7FFF
      FFFFFFFF81FF0000DFBFBFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFF0000
      00000000000000000000000000000000000000000000}
  end
  object ColorDialog1: TColorDialog
    Left = 639
    Top = 158
  end
  object BrushPopupMenu: TPopupMenu
    Images = BtnImgList
    Left = 308
    Top = 209
    object BrushChsColItem: TMenuItem
      Caption = #32972#26223#22615#12426#33394#12398#36984#25246'(&C)'
      OnClick = BrushPanelClick
    end
    object BrushChsBgColItem: TMenuItem
      Caption = #26082#23384#32972#26223#33394#12363#12425#22615#12426#33394#12434#36984#25246'(&A)'
      OnClick = BrushChsBgColItemClick
    end
    object Brush_s2: TMenuItem
      Caption = '-'
    end
    object BrushSelBgColItem: TMenuItem
      Action = EditSelBgCol
    end
    object Brush_s1: TMenuItem
      Caption = '-'
    end
    object BrushRetItem: TMenuItem
      Action = ToolNrm
    end
  end
  object BgColPopupMenu: TPopupMenu
    AutoHotkeys = maManual
    OwnerDraw = True
    Left = 308
    Top = 278
  end
  object ActionList1: TActionList
    Images = BtnImgList
    OnExecute = ActionList1Execute
    Left = 435
    Top = 102
    object FileNew: TAction
      Category = #12501#12449#12452#12523
      Caption = #26032#35215#20316#25104'(&N)'
      HelpContext = 5
      HelpType = htContext
      Hint = #26032#35215#20316#25104
      ImageIndex = 0
      OnExecute = FileNewExecute
    end
    object FileOpen: TAction
      Category = #12501#12449#12452#12523
      Caption = #38283#12367'(&O)...'
      HelpContext = 5
      HelpType = htContext
      Hint = #38283#12367
      ImageIndex = 1
      OnExecute = FileOpenExecute
      OnUpdate = CanOpenUpdate
    end
    object FileMerge: TAction
      Category = #12501#12449#12452#12523
      Caption = #12510#12540#12472'(&M)...'
      HelpContext = 5
      HelpType = htContext
      OnExecute = FileMergeExecute
      OnUpdate = EditChildUpdate
    end
    object FileSave: TAction
      Category = #12501#12449#12452#12523
      Caption = #19978#26360#12365#20445#23384'(&S)'
      HelpContext = 5
      HelpType = htContext
      Hint = #20445#23384
      ImageIndex = 2
      OnExecute = FileSaveExecute
      OnUpdate = FileSaveUpdate
    end
    object FileSaveAs: TAction
      Category = #12501#12449#12452#12523
      Caption = #21517#21069#12434#20184#12369#12390#20445#23384'(&A)...'
      HelpContext = 5
      HelpType = htContext
      OnExecute = FileSaveAsExecute
      OnUpdate = ChildUpdate
    end
    object FileClose: TAction
      Category = #12501#12449#12452#12523
      Caption = #38281#12376#12427'(&C)'
      HelpContext = 5
      HelpType = htContext
      OnExecute = FileCloseExecute
      OnUpdate = ChildUpdate
    end
    object FileAllSaveEnd: TAction
      Category = #12501#12449#12452#12523
      Caption = #20840#12390#20445#23384#12375#12390#32066#20102'(&Z)'
      HelpContext = 5
      HelpType = htContext
      OnExecute = FileAllSaveEndExecute
    end
    object TextImport: TAction
      Category = #12501#12449#12452#12523
      Caption = #12486#12461#12473#12488#21462#12426#36796#12415'(&R)...'
      HelpContext = 5
      HelpType = htContext
      Hint = #26029#29255#12398#20869#23481#12434#12486#12461#12473#12488#12501#12449#12452#12523#12363#12425#35501#12415#36796#12415#12414#12377
      OnExecute = TextImportExecute
      OnUpdate = EditChildUpdate
    end
    object TextExport: TAction
      Category = #12501#12449#12452#12523
      Caption = #12486#12461#12473#12488#26360#12365#20986#12375'(&W)...'
      HelpContext = 5
      HelpType = htContext
      Hint = #26029#29255#12398#20869#23481#12434#12486#12461#12473#12488#12501#12449#12452#12523#12504#26360#12365#20986#12375#12414#12377
      OnExecute = TextExportExecute
      OnUpdate = ChildUpdate
    end
    object CsvExport: TAction
      Category = #12501#12449#12452#12523
      Caption = #26029#29255#12434'CSV'#12501#12449#12452#12523#20986#21147'(&C)...'
      HelpContext = 5
      HelpType = htContext
      OnExecute = CsvExportExecute
      OnUpdate = ChildUpdate
    end
    object TsvExport: TAction
      Category = #12501#12449#12452#12523
      Caption = #26029#29255#12434'TAB'#21306#20999#12426#12501#12449#12452#12523#20986#21147'(&T)...'
      HelpContext = 5
      HelpType = htContext
      OnExecute = TsvExportExecute
      OnUpdate = ChildUpdate
    end
    object FilePrint: TAction
      Category = #12501#12449#12452#12523
      Caption = #21360#21047'(&P)...'
      HelpContext = 5
      HelpType = htContext
      Hint = #21360#21047
      ImageIndex = 3
      OnExecute = FliePrintExecute
      OnUpdate = FilePrintUpdate
    end
    object FileImgOut: TAction
      Category = #12501#12449#12452#12523
      Caption = #30011#20687#12392#12375#12390#20445#23384'(&I)...'
      HelpContext = 5
      HelpType = htContext
      OnExecute = FileImgOutExecute
      OnUpdate = EditChildUpdate
    end
    object FileExit1: TFileExit
      Category = #12501#12449#12452#12523
      Caption = #32066#20102'(&X)'
      HelpContext = 5
      HelpType = htContext
      Hint = #32066#20102'|'#12450#12503#12522#12465#12540#12471#12519#12531#12434#32066#20102#12377#12427
    end
    object EditUndo: TAction
      Category = #32232#38598
      Caption = #20803#12395#25147#12377'(&U)'
      HelpContext = 6
      HelpType = htContext
      Hint = #20803#12395#25147#12377
      ImageIndex = 13
      ShortCut = 16474
      OnExecute = EditUndoExecute
      OnUpdate = UndoPosUpdate
    end
    object EditRedo: TAction
      Category = #32232#38598
      Caption = #12420#12426#30452#12375
      HelpContext = 6
      HelpType = htContext
      Hint = #12420#12426#30452#12375
      ImageIndex = 14
      OnExecute = EditRedoExecute
      OnUpdate = RedoPosUpdate
    end
    object EditCut: TAction
      Category = #32232#38598
      Caption = #20999#12426#21462#12426'(&T)'
      HelpContext = 6
      HelpType = htContext
      Hint = #20999#12426#21462#12426'|'#36984#25246#20013#12398#26029#29255#12434#20999#12426#21462#12387#12390#65398#65391#65412#65418#65438#65391#65420#65383#12408'('#65411#65399#65405#65412#12399#65400#65432#65391#65420#65439#65422#65438#65392#65412#65438#12408#12418')'
      ImageIndex = 4
      ShortCut = 16472
      OnExecute = EditCutExecute
      OnUpdate = EditSelUpdate
    end
    object EditCopy: TAction
      Category = #32232#38598
      Caption = #12467#12500#12540'(&C)'
      HelpContext = 6
      HelpType = htContext
      Hint = #12467#12500#12540'|'#36984#25246#20013#12398#26029#29255#12434#12467#12500#12540#12375#12390#12459#12483#12488#12496#12483#12501#12449#12408'('#65411#65399#65405#65412#12399#65400#65432#65391#65420#65439#65422#65438#65392#65412#65438#12408#12418')'
      ImageIndex = 5
      ShortCut = 16451
      OnExecute = EditCopyExecute
      OnUpdate = SelUpdate
    end
    object EditPaste: TAction
      Category = #32232#38598
      Caption = #36028#12426#20184#12369'(&P)'
      HelpContext = 6
      HelpType = htContext
      Hint = #36028#12426#20184#12369'|'#65398#65391#65412#65418#65438#65391#65420#65383#12398#20869#23481#12434#36028#12426#20184#12369#12414#12377
      ImageIndex = 6
      ShortCut = 16470
      OnExecute = EditPasteExecute
      OnUpdate = EditPasteUpdate
    end
    object EditPasteEx: TAction
      Category = #32232#38598
      Caption = #12510#12454#12473#20301#32622#12395#36028#12426#20184#12369'(&H)'
      HelpContext = 6
      HelpType = htContext
      Hint = #12459#12483#12488#12496#12483#12501#12449#12398#20869#23481#12434#12510#12454#12473#12459#12540#12477#12523#20301#32622#12395#36028#12426#20184#12369#12414#12377
      OnExecute = EditPasteExExecute
      OnUpdate = EditPasteExUpdate
    end
    object EditNew: TAction
      Category = #32232#38598
      Caption = #26032#35215#26029#29255'(&N)'
      HelpContext = 6
      HelpType = htContext
      Hint = #26032#35215#26029#29255'|'#26032#12383#12395#26029#29255#12434#20316#12426#12414#12377
      ImageIndex = 7
      OnExecute = EditNewExecute
      OnUpdate = EditChildUpdate
    end
    object EditNewClip: TAction
      Category = #32232#38598
      Caption = #12463#12522#12483#12503#12508#12540#12489#12363#12425#26032#35215'(&B)'
      HelpContext = 6
      HelpType = htContext
      Hint = #12463#12522#12483#12503#12508#12540#12489#12398#12486#12461#12473#12488#20869#23481#12363#12425#26032#12383#12395#26029#29255#12434#20316#12426#12414#12377
      OnExecute = EditNewClipExecute
      OnUpdate = NewClipUpdate
    end
    object EditDelete: TAction
      Category = #32232#38598
      Caption = #21066#38500'(&D)'
      HelpContext = 6
      HelpType = htContext
      Hint = #36984#25246#20013#12398#26029#29255#12434#21066#38500#12375#12414#12377
      OnExecute = EditDeleteExecute
      OnUpdate = EditSelUpdate
    end
    object EditSelAll: TAction
      Category = #32232#38598
      Caption = #20840#12390#36984#25246'(&A)'
      HelpContext = 6
      HelpType = htContext
      Hint = #20840#12390#12398#26029#29255#12434#36984#25246#12375#12414#12377
      OnExecute = EditSelAllExecute
      OnUpdate = ChildUpdate
    end
    object EditSelBgCol: TAction
      Category = #32232#38598
      Caption = #32972#26223#33394#12391#36984#25246'(&Q)'
      HelpContext = 6
      HelpType = htContext
      Hint = #25351#23450#12375#12383#32972#26223#33394#12398#26029#29255#12434#36984#25246#12375#12414#12377
      OnExecute = EditSelBgColExecute
      OnUpdate = ChildUpdate
    end
    object EditGather: TAction
      Category = #32232#38598
      Caption = #38598#21512'(&S)'
      HelpContext = 6
      HelpType = htContext
      Hint = #38598#21512'|'#36984#25246#20013#12398#26029#29255#12434#38598#21512#12373#12379#12414#12377
      ImageIndex = 8
      OnExecute = EditGatherExecute
      OnUpdate = EditMultiSelUpdate
    end
    object EditSpread: TAction
      Category = #32232#38598
      Caption = #23637#38283'(&X)'
      HelpContext = 6
      HelpType = htContext
      Hint = #23637#38283'|'#36984#25246#20013#12398#26029#29255#12434#36969#24403#12394#31684#22258#12395#12402#12429#12370#12390#20006#12409#12414#12377
      ImageIndex = 9
      OnExecute = EditSpreadExecute
      OnUpdate = EditMultiSelUpdate
    end
    object EditStretch: TAction
      Category = #32232#38598
      Caption = #20280#32302'(&Y)...'
      HelpContext = 6
      HelpType = htContext
      Hint = #20280#32302'|'#36984#25246#20013#26029#29255#12398#12402#12429#12364#12426#20855#21512#31561#12434#35519#25972#12375#12414#12377
      ImageIndex = 10
      OnExecute = EditStretchExecute
      OnUpdate = EditMultiSelUpdate
    end
    object EditGrpStretch: TAction
      Category = #32232#38598
      Caption = #12464#12523#12540#12503#20280#32302'(&W)...'
      HelpContext = 6
      HelpType = htContext
      Hint = #12464#12523#12540#12503#20280#32302'|'#12464#12523#12540#12503#12398#12402#12429#12364#12426#20855#21512#12434#35519#25972#12375#12414#12377
      ImageIndex = 11
      OnExecute = EditGrpStretchExecute
      OnUpdate = EditGrpStretchUpdate
    end
    object EditAway: TAction
      Category = #32232#38598
      Caption = #29255#20184#12369'(&K)'
      HelpContext = 6
      HelpType = htContext
      Hint = #29255#20184#12369'|'#36984#25246#20013#26029#29255#12434#38533#12395#29255#20184#12369#12414#12377
      ImageIndex = 12
      OnExecute = EditAwayExecute
      OnUpdate = EditSelUpdate
    end
    object EditMovePos: TAction
      Category = #32232#38598
      Caption = #12510#12454#12473#20301#32622#12395#31227#21205'(&O)'
      OnExecute = EditMovePosExecute
      OnUpdate = EditSelUpdate
    end
    object EditTrim: TAction
      Category = #32232#38598
      Caption = #12488#12522#12511#12531#12464
      HelpContext = 6
      HelpType = htContext
      OnExecute = EditTrimExecute
      OnUpdate = EditSelUpdate
    end
    object EditFind: TAction
      Category = #32232#38598
      Caption = #26908#32034'(&F)...'
      HelpContext = 6
      HelpType = htContext
      Hint = #26908#32034'|'#25351#23450#12375#12383#35486#12434#21547#12416#26029#29255#12434#35211#12388#12369#12390#36984#25246#12375#12414#12377
      ImageIndex = 15
      OnExecute = EditFindExecute
      OnUpdate = ChildUpdate
    end
    object EditPrevSel: TAction
      Category = #32232#38598
      Caption = #21069#12398#36984#25246#26029#29255#12408
      HelpContext = 6
      HelpType = htContext
      Hint = #19968#12388#21069#12398#36984#25246#26029#29255#12408'|'#19968#12388#21069#12398#36984#25246#20013#26029#29255#12434#30011#38754#12398#20013#22830#12408#25345#12387#12390#12365#12414#12377
      ImageIndex = 16
      OnExecute = EditPrevSelExecute
      OnUpdate = PrevSelUpdate
    end
    object EditNextSel: TAction
      Category = #32232#38598
      Caption = #24460#12398#36984#25246#26029#29255#12408
      HelpContext = 6
      HelpType = htContext
      Hint = #19968#12388#24460#12398#36984#25246#26029#29255#12408'|'#19968#12388#24460#12398#36984#25246#20013#26029#29255#12434#30011#38754#12398#20013#22830#12408#25345#12387#12390#12365#12414#12377
      ImageIndex = 17
      OnExecute = EditNextSelExecute
      OnUpdate = NextSelUpdate
    end
    object EditProperty: TAction
      Category = #32232#38598
      Caption = #12503#12525#12497#12486#12451'(&R)...'
      HelpContext = 6
      HelpType = htContext
      Hint = #26029#29255#12398#20869#23481#12289#12464#12523#12540#12503#12289#12469#12452#12474#12289#33394#31561#12434#22793#26356#12375#12414#12377
      OnExecute = EditPropertyExecute
      OnUpdate = EditPropertyUpdate
    end
    object GoJump: TAction
      Category = #12472#12515#12531#12503
      Caption = #12472#12515#12531#12503'(&J)'
      HelpContext = 7
      HelpType = htContext
      Hint = #36984#25246#20013#26029#29255#12398#12522#12531#12463#20808#12408#39131#12403#12414#12377
      OnExecute = GoJumpExecute
      OnUpdate = SelUpdate
    end
    object GoBack: TAction
      Category = #12472#12515#12531#12503
      Caption = #25147#12427'(&B)'
      HelpContext = 7
      HelpType = htContext
      Hint = #25147#12427'|'#12472#12515#12531#12503#23653#27508#12398#19968#12388#21069#12398#12454#12451#12531#12489#12454#12395#25147#12426#12414#12377
      ImageIndex = 18
      OnExecute = GoBackExecute
      OnUpdate = GoBackUpdate
    end
    object GoForward: TAction
      Category = #12472#12515#12531#12503
      Caption = #36914#12416'(&F)'
      HelpContext = 7
      HelpType = htContext
      Hint = #36914#12416'|'#12472#12515#12531#12503#23653#27508#12398#27425#12398#12454#12451#12531#12489#12454#12395#36914#12415#12414#12377
      ImageIndex = 19
      OnExecute = GoForwardExecute
      OnUpdate = GoForwardUpdate
    end
    object GoHome: TAction
      Category = #12472#12515#12531#12503
      Caption = #12507#12540#12512'(&H)'
      HelpContext = 7
      HelpType = htContext
      Hint = #12507#12540#12512'|'#12507#12540#12512#12454#12451#12531#12489#12454#12434#38283#12365#12414#12377
      ImageIndex = 20
      OnExecute = GoHomeExecute
      OnUpdate = GoHomeUpdate
    end
    object ToolNrm: TAction
      Category = #12484#12540#12523
      Caption = #36890#24120#32232#38598'(&N)'
      HelpContext = 8
      HelpType = htContext
      Hint = #36890#24120#32232#38598
      ImageIndex = 21
      OnExecute = ToolNormalExecute
      OnUpdate = ChildUpdate
    end
    object ToolGruop: TAction
      Category = #12484#12540#12523
      Caption = #12464#12523#12540#12503#21270'(&G)'
      HelpContext = 8
      HelpType = htContext
      Hint = #12464#12523#12540#12503#21270
      ImageIndex = 28
      OnExecute = ToolGroupExecute
      OnUpdate = EditChildUpdate
    end
    object ToolGLine: TAction
      Category = #12484#12540#12523
      Caption = #38306#20418#32218'(&L)'
      HelpContext = 8
      HelpType = htContext
      Hint = #26029#29255#12414#12383#12399#12464#12523#12540#12503#38306#20418#32218
      ImageIndex = 22
      OnExecute = ToolGLineExecute
      OnUpdate = EditChildUpdate
    end
    object ToolBrush: TAction
      Category = #12484#12540#12523
      Caption = #33394#22615#12426'(&B)'
      HelpContext = 8
      HelpType = htContext
      Hint = #26029#29255#32972#26223#12420#12464#12523#12540#12503#26528#12398#33394#22793#26356
      ImageIndex = 23
      OnExecute = ToolBrushExecute
      OnUpdate = EditChildUpdate
    end
    object ToolFindEx: TAction
      Category = #12484#12540#12523
      Caption = #25313#24373#26908#32034'(&Q)'
      HelpContext = 35
      HelpType = htContext
      OnExecute = ToolFindExExecute
    end
    object RubberLay: TAction
      Category = #12484#12540#12523
      Caption = #28014#36938#37197#32622'(&F)'
      HelpContext = 37
      HelpType = htContext
      OnExecute = RubberLayExecute
    end
    object ShowGrpLst: TAction
      Category = #34920#31034
      Caption = #12464#12523#12540#12503#12522#12473#12488'(&G)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowGrpLstExecute
      OnUpdate = ShowGrpLstUpdate
    end
    object ShowFrgGrid: TAction
      Category = #34920#31034
      Caption = #26029#29255#12522#12473#12488'(&W)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowFrgGridExecute
    end
    object ShowMapViewer: TAction
      Category = #34920#31034
      Caption = #20840#20307#22259'(&V)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowMapViewerExecute
    end
    object ShowNamePlate: TAction
      Category = #34920#31034
      Caption = #34920#26413'(&N)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowNamePlateExecute
    end
    object ShowLinkMark: TAction
      Category = #34920#31034
      Caption = #12522#12531#12463#12510#12540#12463'(&L)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowLinkMarkExecute
    end
    object ShowGFrame: TAction
      Category = #34920#31034
      Caption = #12464#12523#12540#12503#26528'(&F)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowGFrameExecute
    end
    object ShowGLine: TAction
      Category = #34920#31034
      Caption = #12464#12523#12540#12503#38306#20418#32218'(&R)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowGLineExecute
    end
    object ShowFLine: TAction
      Category = #34920#31034
      Caption = #26029#29255#38306#20418#32218'(&C)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowFLineExecute
    end
    object ShowDivLine: TAction
      Category = #34920#31034
      Caption = #30011#38754#20998#21106#32218'(&D)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowDivLineExecute
    end
    object ShowPosGrid: TAction
      Category = #34920#31034
      Caption = #37197#32622#12464#12522#12483#12489'(&P)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowPosGridExecute
    end
    object ShowStatusBar: TAction
      Category = #34920#31034
      Caption = #12473#12486#12540#12479#12473#12496#12540'(&S)'
      HelpContext = 9
      HelpType = htContext
      OnExecute = ShowStatusBarExecute
    end
    object WindowTileH: TWindowTileHorizontal
      Category = #12454#12451#12531#12489#12454
      Caption = #32294#12395#20006#12409#12390#34920#31034'(&T)'
      HelpContext = 10
      HelpType = htContext
      ImageIndex = 24
      OnExecute = WindowTileHExecute
      OnUpdate = ChildUpdate
    end
    object WindowTileV: TWindowTileVertical
      Category = #12454#12451#12531#12489#12454
      Caption = #27178#12395#20006#12409#12390#34920#31034'(&H)'
      HelpContext = 10
      HelpType = htContext
      ImageIndex = 25
      OnExecute = WindowTileVExecute
      OnUpdate = ChildUpdate
    end
    object WindowCascade: TWindowCascade
      Category = #12454#12451#12531#12489#12454
      Caption = #37325#12397#12390#34920#31034'(&C)'
      HelpContext = 10
      HelpType = htContext
      ImageIndex = 26
      OnExecute = WindowCascadeExecute
      OnUpdate = ChildUpdate
    end
    object WindowArrange: TAction
      Category = #12454#12451#12531#12489#12454
      Caption = #12450#12452#12467#12531#12398#25972#21015'(&A)'
      HelpContext = 10
      HelpType = htContext
      OnExecute = WindowArrangeExecute
      OnUpdate = ChildUpdate
    end
    object WindowActMax: TAction
      Category = #12454#12451#12531#12489#12454
      Caption = #12450#12463#12486#12451#12502#12454#12451#12531#12489#12454#12434#26368#22823#21270'(&X)'
      HelpContext = 10
      HelpType = htContext
      OnExecute = WindowActMaxExecute
      OnUpdate = ChildUpdate
    end
    object OptionSetup: TAction
      Category = #12458#12503#12471#12519#12531
      Caption = #12458#12503#12471#12519#12531#35373#23450'(&O)...'
      HelpContext = 12
      HelpType = htContext
      OnExecute = OptionSetupExecute
    end
    object OptionIndividual: TAction
      Category = #12458#12503#12471#12519#12531
      Caption = #20491#21029#35373#23450'(&S)...'
      HelpContext = 22
      HelpType = htContext
      OnExecute = OptionIndividualExecute
      OnUpdate = ChildUpdate
    end
    object HelpContents: TAction
      Category = #12504#12523#12503
      Caption = #30446#27425'(&C)'
      HelpContext = 11
      HelpType = htContext
      ImageIndex = 27
      OnExecute = HelpContentsExecute
    end
    object HelpHomePage: TAction
      Category = #12504#12523#12503
      Caption = 'Web'#12469#12452#12488#12408'(&W)'
      HelpContext = 11
      HelpType = htContext
      OnExecute = HelpHomePageExecute
    end
    object HelpAbout: TAction
      Category = #12504#12523#12503
      Caption = #12496#12540#12472#12519#12531#24773#22577'(&A)'
      HelpContext = 11
      HelpType = htContext
      OnExecute = HelpAboutExecute
    end
    object ExMovTop: TAction
      Category = '<'#12381#12398#20182'>'
      Caption = #30011#38754#19978#31471#12395#31227#21205
      HelpType = htContext
      OnExecute = ExMoveExecute
    end
    object ExMovBottom: TAction
      Tag = 1
      Category = '<'#12381#12398#20182'>'
      Caption = #30011#38754#19979#31471#12395#31227#21205
      HelpType = htContext
      OnExecute = ExMoveExecute
    end
    object ExMovLeft: TAction
      Tag = 2
      Category = '<'#12381#12398#20182'>'
      Caption = #30011#38754#24038#31471#12395#31227#21205
      HelpType = htContext
      OnExecute = ExMoveExecute
    end
    object ExMovRight: TAction
      Tag = 3
      Category = '<'#12381#12398#20182'>'
      Caption = #30011#38754#21491#31471#12395#31227#21205
      HelpType = htContext
      OnExecute = ExMoveExecute
    end
    object ExMoveCenter: TAction
      Tag = 4
      Category = '<'#12381#12398#20182'>'
      Caption = #30011#38754#20013#22830#12395#31227#21205
      OnExecute = ExMoveExecute
    end
    object ExCopyAnchor: TAction
      Category = '<'#12381#12398#20182'>'
      Caption = #12501#12449#12452#12523#21517'#ID'#30058#21495#12434#12467#12500#12540'(&I)'
      HelpType = htContext
      OnExecute = CopyAnchorActionExecute
      OnUpdate = EditSelOneUpdate
    end
  end
  object ActionList2: TActionList
    Left = 435
    Top = 153
    object DispStatus: TAction
      Caption = 'DispStatus'
      OnExecute = DispStatusUpdate
      OnUpdate = DispStatusUpdate
    end
  end
  object GFrmPopupMenu: TPopupMenu
    OnPopup = GFrmPopupMenuPopup
    Left = 308
    Top = 153
    object GFrmSelItem: TMenuItem
      Caption = #12464#12523#12540#12503#36984#25246'(&S)'
      OnClick = GFrmSelItemClick
    end
    object GFrmRelItem: TMenuItem
      Caption = #12464#12523#12540#12503#35299#38500'(&R)'
      OnClick = GFrmRelItemClick
    end
    object Gfrm_s1: TMenuItem
      Caption = '-'
    end
    object GFrmNameItem: TMenuItem
      Caption = #34920#26413#12434#20184#12369#12427'(&N)'
      OnClick = GFrmNameItemClick
    end
    object GFrmLwidthItem: TMenuItem
      Caption = #26528#32218#12398#22826#12373'(&W)'
      object GFrmLw0Item: TMenuItem
        Caption = #12487#12501#12457#12523#12488'(&D)'
        OnClick = GFrmSetLwItemClick
      end
      object GFrmLw1Item: TMenuItem
        Tag = 1
        Caption = '&1'
        OnClick = GFrmSetLwItemClick
      end
      object GFrmLw2Item: TMenuItem
        Tag = 2
        Caption = '&2'
        OnClick = GFrmSetLwItemClick
      end
      object GFrmLw3Item: TMenuItem
        Tag = 3
        Caption = '&3'
        OnClick = GFrmSetLwItemClick
      end
      object GFrmLw4Item: TMenuItem
        Tag = 4
        Caption = '&4'
        OnClick = GFrmSetLwItemClick
      end
      object GFrmLw5Item: TMenuItem
        Tag = 5
        Caption = '&5'
        OnClick = GFrmSetLwItemClick
      end
      object GFrmLw6Item: TMenuItem
        Tag = 6
        Caption = '&6'
        OnClick = GFrmSetLwItemClick
      end
      object GFrmLw7Item: TMenuItem
        Tag = 7
        Caption = '&7'
        OnClick = GFrmSetLwItemClick
      end
      object GFrmLw8Item: TMenuItem
        Tag = 8
        Caption = '&8'
        OnClick = GFrmSetLwItemClick
      end
      object GFrmLw9Item: TMenuItem
        Tag = 9
        Caption = '&9'
        OnClick = GFrmSetLwItemClick
      end
      object GFrmLw10Item: TMenuItem
        Tag = 10
        Caption = '1&0'
        OnClick = GFrmSetLwItemClick
      end
    end
    object GFrmLcolorItem: TMenuItem
      Caption = #26528#32218#12398#20491#21029#33394#25351#23450'(&C)...'
      OnClick = GFrmLcolorItemClick
    end
    object Gfrm_s2: TMenuItem
      Caption = '-'
    end
    object GFrmRetItem: TMenuItem
      Action = ToolNrm
    end
  end
  object EditPopupMenu2: TPopupMenu
    Left = 206
    Top = 154
    object PopUndoPosItem2: TMenuItem
      Action = EditUndo
    end
    object PopRedoPosItem2: TMenuItem
      Action = EditRedo
    end
    object P2_s1: TMenuItem
      Caption = '-'
    end
    object PopCutItem2: TMenuItem
      Action = EditCut
    end
    object PopCopyItem2: TMenuItem
      Action = EditCopy
    end
    object PopDeleteItem2: TMenuItem
      Action = EditDelete
    end
    object P2_s2: TMenuItem
      Caption = '-'
    end
    object PopAwayItem2: TMenuItem
      Action = EditAway
    end
    object P2_s3: TMenuItem
      Caption = '-'
    end
    object PopFindItem2: TMenuItem
      Action = EditFind
    end
    object PopJumpItem2: TMenuItem
      Action = GoJump
    end
    object IDI1: TMenuItem
      Action = ExCopyAnchor
    end
    object P2_s4: TMenuItem
      Caption = '-'
    end
    object PopPropertyItem2: TMenuItem
      Action = EditProperty
    end
  end
  object ApplicationEvents1: TApplicationEvents
    OnHelp = ApplicationEvents1Help
    OnMessage = ApplicationEvents1Message
    OnModalBegin = ApplicationEvents1ModalBegin
    OnModalEnd = ApplicationEvents1ModalEnd
    Left = 120
    Top = 48
  end
end
