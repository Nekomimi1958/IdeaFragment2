object FragGrid: TFragGrid
  Left = 0
  Top = 0
  HelpContext = 34
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = #26029#29255#12522#12473#12488
  ClientHeight = 455
  ClientWidth = 555
  Color = clBtnFace
  DoubleBuffered = True
  ParentFont = True
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  Scaled = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnDeactivate = FormDeactivate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ListSplitter: TSplitter
    Left = 1
    Top = 0
    Width = 4
    Height = 455
    Beveled = True
    ResizeStyle = rsUpdate
    ExplicitHeight = 452
  end
  object DockPanel: TPanel
    Left = 0
    Top = 0
    Width = 1
    Height = 455
    Align = alLeft
    BevelOuter = bvNone
    DockSite = True
    TabOrder = 0
    OnDockDrop = DockPanelDockDrop
    OnDockOver = DockPanelDockOver
    OnResize = DockPanelResize
    OnUnDock = DockPanelUnDock
  end
  object ListPanel: TPanel
    Left = 5
    Top = 0
    Width = 550
    Height = 455
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 1
    TabOrder = 1
    object FrgLstGrid: TStringGrid
      Left = 1
      Top = 1
      Width = 548
      Height = 453
      Align = alClient
      BorderStyle = bsNone
      ColCount = 7
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect, goThumbTracking]
      TabOrder = 0
      StyleElements = [seBorder]
      OnDblClick = FrgLstGridDblClick
      OnDrawCell = FrgLstGridDrawCell
      OnEnter = FrgLstGridEnter
      OnExit = FrgLstGridExit
      OnKeyDown = FrgLstGridKeyDown
      OnMouseDown = FrgLstGridMouseDown
      ColWidths = (
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
end
