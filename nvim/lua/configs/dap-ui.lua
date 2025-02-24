local dap = require "dap"
local dapui = require "dapui"
dapui.setup()
dap.listeners.after.event_initialized["dapui_config"] = function()
  dapui.open()
end
dap.listeners.before.event_terminated["dapui_config"] = function()
  dapui.close()
end
dap.listeners.before.event_exited["dapui_config"] = function()
  dapui.close()
end
vim.fn.sign_define("DapBreakpoint", {
  linehl = "",
  numhl = "",
  text = "🔴", -- nerdfonts icon here
  texthl = "",
})
vim.fn.sign_define("DapStopped", {
  linehl = "PmenuThumb",
  numhl = "",
  text = "🟢", -- nerdfonts icon here
  texthl = "",
})
