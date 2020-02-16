using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System;

namespace MetroEXControls {
    public struct CharInfo {
        public int x;
        public int y;
        public int width;
        public int height;
        public int xoffset;
        public int yoffset;
        public int xadvance;
    }

    public partial class FontPreviewWnd : Form {
        public Image            mImage;
        public List<char>       mCharsMap;
        public List<CharInfo>   mCharsDB;
        public int              mLineHeight;
        public PointF           mScale = new PointF();

        public FontPreviewWnd() {
            InitializeComponent();
        }

        public void Redraw() {
            this.pnlCanvas.Invalidate();
        }

        private void pnlCanvas_Paint(object sender, PaintEventArgs e) {
            e.Graphics.Clear(Color.Black);
            e.Graphics.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceOver;

            if (mImage == null || mCharsMap.Count == 0 || mCharsDB.Count == 0 || this.txtText.Text.Length == 0) {
                return;
            }

            Size textSize = this.CalculateTextDimensions(this.txtText.Text);
            if (textSize.Width == 0) {
                return;
            }

            Rectangle rc = this.pnlCanvas.ClientRectangle;
            Rectangle textRC = new Rectangle((rc.Width - textSize.Width) / 2, (rc.Height - textSize.Height) / 2, textSize.Width, textSize.Height);

            int lh = (int)(mScale.Y * (float)mLineHeight);

            int numLines = (textSize.Height + 1) / lh;
            for (int i = 0; i <= numLines; ++i) {
                int y = textRC.Top + lh * i;
                e.Graphics.DrawLine(Pens.White, new Point(textRC.Left, y), new Point(textRC.Right, y));
            }

            string text = this.txtText.Text;
            int posY = textRC.Top;
            int posX = textRC.Left;
            foreach (char ch in text) {
                if (ch == '\n') {
                    posY += lh;
                    posX = textRC.Left;
                } else if (ch != '\r') {
                    CharInfo ci = this.InfoFromChar(ch);

                    if (ch != ' ') { // no need to draw whitespace
                        int x = posX + (int)(mScale.X * (float)ci.xoffset);
                        int y = posY + (int)(mScale.Y * (float)ci.yoffset);
                        Rectangle srcRect = new Rectangle(Math.Max(0, (int)(mScale.X * (float)ci.x)), Math.Max(0, (int)(mScale.Y * (float)ci.y)),
                                                          (int)(mScale.X * (float)ci.width), (int)(mScale.Y * (float)ci.height));
                        e.Graphics.DrawImage(mImage, x, y, srcRect, GraphicsUnit.Pixel);
                    }

                    posX += (int)(mScale.X * (float)ci.xadvance);
                }
            }
        }

        private CharInfo InfoFromChar(char ch) {
            //#NOTE_SK: some fonts are ASCII only, mCharsDB.Count == 256 in that case
            bool isAsciiFont = mCharsDB.Count != mCharsMap.Count && mCharsDB.Count == 256;

            int idx = -1;
            if (isAsciiFont) {
                idx = (int)ch;
                if (idx >= 256) {
                    idx = (int)'?';
                }
            } else {
                for (int i = 0; i < mCharsMap.Count; ++i) {
                    if (mCharsMap[i] == ch) {
                        idx = i;
                        break;
                    }
                }
            }

            if (idx != -1) {
                return mCharsDB[idx];
            } else {
                return new CharInfo();
            }
        }

        private Size CalculateTextDimensions(string text) {
            int lh = (int)(mScale.Y * (float)mLineHeight);

            Size result = new Size(0, lh);

            int lineWidth = 0;
            foreach (char ch in text) {
                if (ch == '\n') {
                    result.Height += lh;
                    result.Width = Math.Max(lineWidth, result.Width);
                    lineWidth = 0;
                } else if (ch != '\r') {
                    CharInfo ci = this.InfoFromChar(ch);
                    lineWidth += (int)(mScale.X * (float)ci.xadvance);
                }
            }

            result.Width = Math.Max(lineWidth, result.Width);

            return result;
        }

        private void txtText_TextChanged(object sender, EventArgs e) {
            this.Redraw();
        }
    }
}
