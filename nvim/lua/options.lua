require "nvchad.options"


local o = vim.o
local wo = vim.wo

o.guicursor = 'n-v-c-sm-i-ci-ve:block,r-cr-o:hor20,a:blinkwait300-blinkoff200-blinkon150-nCursor'

wo.relativenumber = true

--for UFO plugin
o.foldcolumn = '0' -- '0' is not bad
o.foldlevel = 99 -- Using ufo provider need a large value, feel free to decrease the value
o.foldlevelstart = 99
o.foldenable = true
vim.keymap.set('n', 'zr', require('ufo').openAllFolds)
vim.keymap.set('n', 'zm', require('ufo').closeAllFolds)
-----------------------

-- vim.lsp.set_log_level("debug") -- for lsp debugging
vim.lsp.set_log_level("off") -- Clang log file was huge ~500mb. So disable it

vim.opt.mps:append("<:>") -- for  jumping betweeen <> like ()


-- vim.cmd([[
-- function!   QuickFixOpenAll()
--     if empty(getqflist())
--         return
--     endif
--     let s:prev_val = ""
--     for d in getqflist()
--         let s:curr_val = bufname(d.bufnr)
--         if (s:curr_val != s:prev_val)
--             exec "edit " . s:curr_val
--         endif
--         let s:prev_val = s:curr_val
--     endfor
-- endfunction
-- ]])
-- vim.api.nvim_set_keymap('n', '<leader>ka' , ':call QuickFixOpenAll()<CR>', { noremap=true, silent=false })


-- o.cursorlineopt ='both' -- to enable cursorline!
