require  "open3"

Dir.foreach(Dir.pwd) do |f|
	if f.include? ".png"
		fn = File.join(Dir.pwd, f)
		File.delete(fn)
	end
end

Dir.foreach(Dir.pwd) do |f|
	if f.include? ".dot"
		Open3.pipeline("dot -Tpng " + f + " -o " + f.chomp(".dot") + ".png")
	end
end