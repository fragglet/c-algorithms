#!/usr/bin/env ruby
# 
# Smart indenting script
#
# Replaces leading whitespace on lines with an appropriate combination
# of tabs and spaces so that code is indented with tabs but in a way
# which is independent of the tab size - ie. the outputted code will
# view properly in any editor regardless of tab size.
#

TAB_SIZE = 8

def tab_out_to(characters)
    tabs = (characters / TAB_SIZE).to_i
    spaces = characters % TAB_SIZE
    ("\t" * tabs) + (" " * spaces)
end

def fix_file(filename)
    infile = File.open(filename, "r")
    outfile = File.open("tmpfile.c", "w")

    bracket_count = 0
    indent_level = 0
    in_block_comment = false

    infile.each_line do |s|
        s = s.chomp

        # expand tabs

	while s =~ /\t/
	    lead_text = $`
	    lead_len = lead_text.length
	    replace_len = TAB_SIZE - (lead_len % TAB_SIZE)
	    s[0, lead_len + 1] = lead_text + " " * replace_len
	end

        # replace leading spaces with the appropriate combination
        # of tabs and spaces

        s = s.sub(/^(\s*)/) do 

            if bracket_count == 0
                indent_level = $1.length
            end

            if $1.length >= indent_level
                tab_out_to(indent_level) + (" " * ($1.length - indent_level))
            else
                " " * $1.length
            end
        end

        outfile.puts s

        # strip out C++ comments

        s = s.sub(/\/\/.*/, '')

        # strip out C-style comments which are all on one line 

        s = s.gsub(/\/\*.*?\*\//, '')

        # end of a block multiline comment?

        if in_block_comment
            s = s.sub(/^.*\*\//) do 
                in_block_comment = false
                ""
            end

            if in_block_comment
                # no end - the entire line is in a block comment

                s = ""
            end
        end

        # beginning of a multiline block comment?

        if !in_block_comment
            s = s.gsub(/\/\*.*$/) do
                in_block_comment = true
                ""
            end
        end

        # now count brackets

        s.gsub(/([\(\)])/) do 
            if $1 == "("
                bracket_count += 1
            else
                bracket_count -= 1
            end
        end
    end

    outfile.close()
    infile.close()

    File.rename(filename, "#{filename}~")
    File.rename("tmpfile.c", filename)
end

ARGV.each do |filename|
    fix_file(filename)
end

