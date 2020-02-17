using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using System.Xml;

namespace MetroEXControls {
    public partial class FilterableTreeView : UserControl {
        public string FilterPlaceholder { get; set; } = "Search here...";
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
            WinApi.SendMessage(FilterTextBox.Handle, WinApi.EM_SETCUEBANNER, 0, FilterPlaceholder);

            mTimer = new Timer();
            mTimer.Interval = FilterTimeout;
            mTimer.Tick += new EventHandler(filterTimer_Tick);
        }

        public bool SearchCheck()
        {
            bool result = false;
            if (File.Exists(Directory.GetCurrentDirectory() + "/settings.mex"))
            {
                try
                {
                    XmlDocument xmlDoc = new XmlDocument();
                    xmlDoc.Load(Directory.GetCurrentDirectory() + "/settings.mex");
                    XmlNodeList parentNode = xmlDoc.GetElementsByTagName("Extraction");
                    foreach (XmlNode childrenNode in parentNode)
                    {
                        if (childrenNode.SelectSingleNode("askSearch").InnerText == "false")
                        {
                            result = false; //settings.mex now in askSearch xml config false, then return false
                        }
                        else
                        {
                            result = true; //settings.mex now in askSearch xml config true, then return true
                        }
                    }
                }
                catch
                {
                    result = false; //node not found in xml or error return false
                }
            }
            else
            {
                result = false; //settings.mex not found, return false
            }
            return result;
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

            if (SearchCheck())
            {
                //alternative search enabled with enter
            }
            else
            {
                Cursor.Current = Cursors.WaitCursor;

                this.treeView.BeginUpdate();
                this.treeView.Nodes.Clear();

                if (string.IsNullOrWhiteSpace(this.filterTextBox.Text))
                {
                    mIsFiltering = false;
                    this.treeView.Nodes.AddRange(mOriginalRootNodes);
                }
                else
                {
                    mIsFiltering = true;
                    for (var i = 0; i < mOriginalRootNodes.Length; ++i)
                    {
                        var rootNode = mOriginalRootNodes[i].Clone() as TreeNode;
                        FilterTreeView(rootNode, this.filterTextBox.Text);
                        this.treeView.Nodes.Add(rootNode);
                        this.treeView.Nodes[i].ExpandAll();
                    }
                }

                this.treeView.EndUpdate();

                Cursor.Current = Cursors.Arrow;
            }
        }

        //search
        private void filterTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (SearchCheck())
            {
                //when search with enter enabled use code
                if (e.KeyCode == Keys.Enter)
                {
                    Cursor.Current = Cursors.WaitCursor;

                    this.treeView.BeginUpdate();
                    this.treeView.Nodes.Clear();

                    if (string.IsNullOrWhiteSpace(this.filterTextBox.Text))
                    {
                        mIsFiltering = false;
                        this.treeView.Nodes.AddRange(mOriginalRootNodes);
                    }
                    else
                    {
                        mIsFiltering = true;
                        for (var i = 0; i < mOriginalRootNodes.Length; ++i)
                        {
                            var rootNode = mOriginalRootNodes[i].Clone() as TreeNode;
                            FilterTreeView(rootNode, this.filterTextBox.Text);
                            this.treeView.Nodes.Add(rootNode);
                            this.treeView.Nodes[i].ExpandAll();
                        }
                    }

                    this.treeView.EndUpdate();

                    Cursor.Current = Cursors.Arrow;
                }
            }
            else
            {
                //when search with enter disabled use original code
            }
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

            if (SearchCheck())
            {
                //alternative search enabled with enter
            }
            else
            {
                if (mOriginalRootNodes != null && mOriginalRootNodes.Length > 0)
                {
                    mTimer.Stop();
                    mTimer.Start();
                }
            }
        }

        public bool FindAndSelect(string text, string[] extensions) {
            if (mOriginalRootNodes == null) {
                Initialize();
            }

            var textParts = text.Split('\\');

            TreeNode node = null;
            foreach(var rootNode in mOriginalRootNodes) {
                TreeNode foundNode = null;

                var nodeToSearch = rootNode;
                for (int i = 0; i < textParts.Length; i++) {
                    foundNode = this.FindNode(nodeToSearch, textParts[i]);

                    if (i == textParts.Length - 1) {
                        if(extensions != null) {
                            for (int j = 0; j < extensions.Length; j++) {
                                foundNode = this.FindNode(nodeToSearch, textParts[i] + extensions[j]);

                                if (foundNode != null) {
                                    break;
                                }
                            }
                        }
                    } else if(foundNode != null) {
                        foundNode.Expand();
                    }

                    nodeToSearch = foundNode;
                }

                if(foundNode != null)
                {
                    node = foundNode;
                    break;
                }
            }

            if (node != null) {
                this.TreeView.SelectedNode = node;

                return true;
            }

            return false;
        }

        private TreeNode FindNode(TreeNode parent, string text) {
            var term = text.ToUpper();

            for (int i = 0; i < parent.Nodes.Count; i++) {
                if (parent.Nodes[i].Text.ToUpper() == term) {
                    return parent.Nodes[i];
                }
            }

            return null;
        }
    }
}
