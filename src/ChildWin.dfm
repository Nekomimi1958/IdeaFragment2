object MDIChild: TMDIChild
  Left = 0
  Top = 0
  HelpContext = 1
  ClientHeight = 228
  ClientWidth = 313
  Color = clBtnFace
  DoubleBuffered = True
  ParentFont = True
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  Visible = True
  OnActivate = FormActivate
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDblClick = FormDblClick
  OnDestroy = FormDestroy
  OnDeactivate = FormDeactivate
  OnKeyDown = FormKeyDown
  OnKeyUp = FormKeyUp
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object PopPosShape: TShape
    Left = 149
    Top = 106
    Width = 15
    Height = 16
    Brush.Style = bsClear
    ParentShowHint = False
    Pen.Color = clRed
    Shape = stCircle
    ShowHint = False
    Visible = False
  end
  object SaveDialog: TSaveDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 16
    Top = 8
  end
  object ScrollTimer: TTimer
    Enabled = False
    OnTimer = ScrollTimerTimer
    Left = 16
    Top = 64
  end
end
