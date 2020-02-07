using System;
using System.Drawing;
using System.Windows.Forms;

namespace MetroEXControls {
    public partial class ImagePanel : UserControl {
        private Brush   mBackgroundBrush = null;
        private Image   mImage = null;
        private Point   mStartMousePos;
        private Point   mStartScrollPosition;
        private bool    mPanning = false;
        private bool    mTransparencyEnabled = true;

        #region Delegates
        public delegate void OnPreDrawDelegate(System.Drawing.Graphics g);
        public delegate void OnPostDrawDelegate(System.Drawing.Graphics g);

        public OnPreDrawDelegate OnPreDraw { get; set; } = null;
        public OnPostDrawDelegate OnPostDraw { get; set; } = null;
        #endregion

        public Image Image {
            get {
                return this.mImage;
            }
            set {
                this.mImage = value;
                this.AdjustScrolling();
                this.Invalidate();
            }
        }

        public bool TransparencyEnabled {
            get {
                return mTransparencyEnabled;
            }
            set {
                if (mTransparencyEnabled != value) {
                    mTransparencyEnabled = value;
                    this.Invalidate();
                }
            }
        }

        public ImagePanel() {
            Bitmap bmp = new Bitmap(16, 16);
            for (int y = 0; y < bmp.Height; ++y) {
                for (int x = 0; x < bmp.Width; ++x) {
                    bool isWhite = (x < 8 && y < 8) || (x >= 8 && y >= 8);
                    bmp.SetPixel(x, y, isWhite ? Color.LightGray : Color.DarkGray);
                }
            }
            mBackgroundBrush = new TextureBrush(bmp);

            InitializeComponent();

            this.SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw, true);
            this.SetStyle(ControlStyles.Selectable, false);
            this.UpdateStyles();
        }

        public int GetDrawOffsetX() {
            int leftWithOffset = this.Padding.Left;
            if (this.AutoScroll) {
                leftWithOffset += this.AutoScrollPosition.X;
            }

            return leftWithOffset;
        }

        public int GetDrawOffsetY() {
            int topWithOffset = this.Padding.Top;
            if (this.AutoScroll) {
                topWithOffset += this.AutoScrollPosition.Y;
            }

            return topWithOffset;
        }

        protected override void OnPaint(PaintEventArgs e) {
            this.OnPreDraw?.Invoke(e.Graphics);

            int left = this.Padding.Left;
            int top = this.Padding.Top;

            int leftWithOffset = left;
            int topWithOffset = top;
            if (this.AutoScroll) {
                leftWithOffset += this.AutoScrollPosition.X;
                topWithOffset += this.AutoScrollPosition.Y;
            }

            // background
            if (mImage != null && mTransparencyEnabled) {
                int width = Math.Min(mImage.Width, this.ClientRectangle.Width);
                int height = Math.Min(mImage.Height, this.ClientRectangle.Height);

                e.Graphics.FillRectangle(mBackgroundBrush, left, top, width, height);
            } else {
                e.Graphics.FillRectangle(Brushes.White, this.ClientRectangle);
            }

            // image
            if (mImage != null) {
                if (!mTransparencyEnabled) {
                    e.Graphics.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                } else {
                    e.Graphics.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                }

                e.Graphics.DrawImageUnscaled(mImage, leftWithOffset, topWithOffset);
            }

            this.OnPostDraw?.Invoke(e.Graphics);

            // borders
            ControlPaint.DrawBorder(e.Graphics, this.ClientRectangle, this.ForeColor, ButtonBorderStyle.Solid);
        }

        protected override void OnScroll(ScrollEventArgs e) {
            base.OnScroll(e);

            this.Invalidate();
        }

        protected override void OnMouseMove(MouseEventArgs e) {
            base.OnMouseMove(e);

            if (e.Button == MouseButtons.Left) {
                if (!mPanning) {
                    mStartMousePos = e.Location;
                    this.SwitchPanning(true);
                }

                if (mPanning) {
                    int x = -(mStartScrollPosition.X - (mStartMousePos.X - e.Location.X));
                    int y = -(mStartScrollPosition.Y - (mStartMousePos.Y - e.Location.Y));

                    this.UpdateScrollPosition(new Point(x, y));
                }
            }
        }

        protected override void OnMouseUp(MouseEventArgs e) {
            base.OnMouseUp(e);

            if (mPanning) {
                this.SwitchPanning(false);
            }
        }

        private void SwitchPanning(bool isPanning) {
            if (isPanning != mPanning) {
                mPanning = isPanning;
                mStartScrollPosition = this.AutoScrollPosition;
                this.Cursor = isPanning ? Cursors.SizeAll : Cursors.Default;
            }
        }

        private void UpdateScrollPosition(Point position) {
            this.AutoScrollPosition = position;
            this.OnScroll(new ScrollEventArgs(ScrollEventType.ThumbPosition, 0));
            this.Invalidate();
        }

        private void AdjustLayout() {
            if (this.AutoSize) {
                this.AdjustSize();
            } else if (this.AutoScroll) {
                this.AdjustScrolling();
            }
        }

        private void AdjustSize() {
            if (this.AutoSize && this.Dock == DockStyle.None) {
                this.Size = this.PreferredSize;
            }
        }

        private void AdjustScrolling() {
            if (this.AutoScroll && mImage != null) {
                this.AutoScrollMinSize = mImage.Size;
            }
        }
    }
}
