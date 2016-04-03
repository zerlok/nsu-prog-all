"""This module reads multiple results of SF-56 spectrum files and shows it in one plot.
   X axis - wave length (nm).
   Y axis - intensity (in percents)."""


from sys import argv, flags as sysflags
from sf56spectrum import read_sf, validate_sf_files

import pyqtgraph as pyg
from pyqtgraph.Qt import QtGui, QtCore


WINDOW_TITLE = "SF-56 Spectrums View"


def generate_palettes(n):
	'''Generates n RGB palettes.'''
	if not n:
		return []
	
	p_size = 256
	p_max = 250
	p_min = 0
	p_step = (p_max - p_min) / n
	
	return [((p_size/n*17 - x) % p_size, (x + p_size/n*31) % p_size, (x + p_size/n*43) % p_size)
			for x in xrange(p_min, p_max, p_step)]


def create_plot(window):
	plot = window.addPlot(title="Spectrums")
	plot.showGrid(x=True, y=True)
	plot.setLabel('left', "Intensity", units="%")
	plot.setLabel('bottom', "Wave Length (nm)")
	plot.addLegend()
	
	return plot


def show_spectrums(*spectrums):
	# Create a Qt application and show the spectrums.
	app = QtGui.QApplication([])
	window = pyg.GraphicsWindow(title=WINDOW_TITLE)
	window.resize(1000, 600)
	pyg.setConfigOptions(antialias=True)
	
	palette = generate_palettes(len(spectrums))
	plot_view = create_plot(window)

	# Add each spectrum to plot view.
	for i in xrange(len(spectrums)):
		plot_view.plot(
				name = spectrums[i].name,
				x = spectrums[i].axis_x,
				y = spectrums[i].axis_y,
				pen = palette[i],
		)

	# Run Qt app.
	app.exec_()


def main(argv):
	# Taken from pyqtgraph module examples how to run Qt app.
	if (sysflags.interactive == 1) and hasattr(QtCore, 'PYQT_VERSION'):
		print "Can't run the application: unknown error!"
		print "Interactive: %s" % sysflags.interactive
		print "QtCore: %s" % QtCore
		return
	
	if not validate_sf_files(argv[1:]):
		return
	
	# Read .sf files into spectrums.
	spectrums = [read_sf(filename) for filename in argv[1:]]
	show_spectrums(*spectrums)


if __name__ == '__main__':
	if len(argv) == 1:
		print "Usage:", __file__, "[FILES...]"
		print __doc__
		exit(0)
	
	main(argv)
