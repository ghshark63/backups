--snipmatee format
vim.g.snipmate_snippets_path = vim.fn.stdpath "config" .. "/snippets/snipmate"
require("luasnip.loaders.from_snipmate").load()
require("luasnip.loaders.from_snipmate").lazy_load { paths = vim.g.snipmate_snippets_path or "" }

-- uncomment if "doc" friendly snippets don't work
--
-- require("luasnip").filetype_extend("cpp", {"cppdoc"})
-- require("luasnip").filetype_extend("python", {"pydoc"})

vim.api.nvim_create_autocmd("InsertLeave", {
  callback = function()
    if
      require("luasnip").session.current_nodes[vim.api.nvim_get_current_buf()]
      and not require("luasnip").session.jump_active
    then
      require("luasnip").unlink_current()
    end
  end,
})
