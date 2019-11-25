﻿using PythonSTGVisualEditor.SpecialNode;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualBasic;

namespace PythonSTGVisualEditor
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void fileFToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void toolStripStatusLabel1_Click(object sender, EventArgs e)
        {

        }

        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void 退出XToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void ToolTree_DoubleClickEvent(object sender, EventArgs e)
        {
            TreeView rScriptContext = scriptContext;
            TreeView treeView = (TreeView)sender;

            TreeNode toolSelectNode = treeView.SelectedNode;
            TreeNode toolSelectParent = treeView.SelectedNode.Parent;
            
            TreeNode scriptSelectNode = rScriptContext.SelectedNode;
            if (toolSelectParent != null) {//No root node
                switch (toolSelectNode.Text) {
                    case "Stage":
                        if (scriptSelectNode != null) {

                            if (scriptSelectNode.Text == scriptContext.Nodes[0].Text)
                            {
                                string varName = Interaction.InputBox("输入关卡变量名:", "Input value");
                                string displayName = Interaction.InputBox("输入关卡显示名称:", "Input value");
                                StageNode newNode = new StageNode(varName, displayName);
                                scriptSelectNode.Nodes.AddRange(new TreeNode[] {
                                 newNode});
                            }
                            else {
                                MessageBox.Show("只能在Stage Group节点下新建Stage!", "无法在此处添加节点", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                        }
                        else
                        {
                            MessageBox.Show("未选择节点！", "无法在此处添加节点", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                        break;

                    case "Task":
                        FormTask formTask = new FormTask();
                        formTask.Show();
                        break;

                }
   
            }
            
        }

        private void ScriptTree_DoubleClick(object sender, EventArgs e)
        {
            TreeView scriptContext = (TreeView)sender;
            TreeNode currentNode = scriptContext.SelectedNode;
            if (currentNode != null) {
                if (currentNode is StageNode) {
                    StageNode stageNode = (StageNode)currentNode;
                    string varName = Interaction.InputBox("输入关卡变量名:", "Input value", stageNode.getVarName());
                    string displayName = Interaction.InputBox("输入关卡显示名称:", "Input value", stageNode.getDisplayName());
                    if (varName.Length > 0 && displayName.Length > 0) {
                        stageNode.setNodeInfo(varName, displayName);
                    }
                }
            }
        }

        private void nodeMoveUp_Click(object sender, EventArgs e)
        {
            TreeView rScriptContext = scriptContext;
            TreeNode currentNode = rScriptContext.SelectedNode;
            if (currentNode != null && currentNode.Parent != null) {
                TreeNode parentNode = currentNode.Parent;
                int index = parentNode.Nodes.IndexOf(currentNode);
                if (index > 0) {
                    parentNode.Nodes.Remove(currentNode);
                    parentNode.Nodes.Insert(--index, currentNode);
                }
            }
        }

        private void nodeMoveDown_Click(object sender, EventArgs e)
        {
            TreeView rScriptContext = scriptContext;
            TreeNode currentNode = rScriptContext.SelectedNode;
            if (currentNode != null && currentNode.Parent != null)
            {
                TreeNode parentNode = currentNode.Parent;
                int index = currentNode.Parent.Nodes.IndexOf(currentNode);
                if (index + 1 < parentNode.GetNodeCount(true))
                {
                    parentNode.Nodes.Remove(currentNode);
                    parentNode.Nodes.Insert(++index, currentNode);
                }
            }
        }

        private void 复制CToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }
    }
}
