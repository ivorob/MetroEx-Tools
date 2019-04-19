using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace MetroEXControls {
    public partial class FilterableTreeView : UserControl {
        #region WinAPI bindings
        private const int EM_SETCUEBANNER = 0x1501;
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern Int32 SendMessage(IntPtr hWnd, int msg, int wParam, [MarshalAs(UnmanagedType.LPWStr)]string lParam);
        #endregion

        public int FilterTimeout { get; set; } = 1000;
        public TreeView TreeView { get { return this.treeView; } }
        public TextBox FilterTextBox { get { return this.filterTextBox; } }
        public bool IsFiltering { get { return mIsFiltering; } }

        private Timer mTimer;
        private TreeNode[] mOriginalRootNodes;
        private bool mIsFiltering;

        public FilterableTreeView() {
            InitializeComponent();

            //#NOTE_SK: set placeholder text for better and cooler look ;)
            SendMessage(FilterTextBox.Handle, EM_SETCUEBANNER, 0, "Search here...");

            mTimer = new Timer();
            mTimer.Interval = FilterTimeout;
            mTimer.Tick += new EventHandler(filterTimer_Tick);
        }

        public void Initialize() {
            if (mOriginalRootNodes == null) {
                mOriginalRootNodes = new TreeNode[this.treeView.Nodes.Count];
            } else if(mOriginalRootNodes.Length != this.treeView.Nodes.Count) {
                Array.Resize(ref mOriginalRootNodes, this.treeView.Nodes.Count);
            }

            this.treeView.Nodes.CopyTo(mOriginalRootNodes, 0);
        }

        private void filterTimer_Tick(Object sender, EventArgs e) {
            mTimer.Stop();

            Cursor.Current = Cursors.WaitCursor;

            this.treeView.BeginUpdate();
            this.treeView.Nodes.Clear();

            if (string.IsNullOrWhiteSpace(this.filterTextBox.Text)) {
                mIsFiltering = false;
                this.treeView.Nodes.AddRange(mOriginalRootNodes);
            } else {
                mIsFiltering = true;
                for (var i = 0; i < mOriginalRootNodes.Length; ++i) {
                    var rootNode = mOriginalRootNodes[i].Clone() as TreeNode;
                    FilterTreeView(rootNode, this.filterTextBox.Text);
                    this.treeView.Nodes.Add(rootNode);
                    this.treeView.Nodes[i].ExpandAll();
                }
            }

            this.treeView.EndUpdate();

            Cursor.Current = Cursors.Arrow;
        }

        private void FilterTreeView(TreeNode node, string text) {
            var nodesToRemove = new List<TreeNode>();

            for (int i = 0; i < node.Nodes.Count; ++i) {
                if (node.Nodes[i].Nodes.Count > 0) {
                    FilterTreeView(node.Nodes[i], text);

                    if (node.Nodes[i].Nodes.Count == 0) {
                        nodesToRemove.Add(node.Nodes[i]);
                    }
                } else if (!node.Nodes[i].Text.Contains(text)) {
                    nodesToRemove.Add(node.Nodes[i]);
                }
            }

            for (int i = 0; i < nodesToRemove.Count; ++i) {
                node.Nodes.Remove(nodesToRemove[i]);
            }
        }

        private void filterText_TextChanged(object sender, EventArgs e) {
            if (mOriginalRootNodes == null) {
                Initialize();
            }

            if (mOriginalRootNodes != null && mOriginalRootNodes.Length > 0) {
                mTimer.Stop();
                mTimer.Start();
            }
        }
    }
}
