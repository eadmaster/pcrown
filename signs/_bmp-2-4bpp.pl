#!/usr/bin/perl
#
# bmp-2-4bpp.pl
# Convert bitmap to SEGA Saturn 4bpp graphic format using an external palette.
#
# Version 1.1
#
# Written by Derek Pascarella (ateam)

use strict;

# Store input parameters.
my $input_file = $ARGV[0];
my $palette_file = $ARGV[1];
my $palette_index = $ARGV[2];
my $width = $ARGV[3];
my $height = $ARGV[4];

# Store version number.
my $version_number = "1.1";

# Store usage text.
my $usage_header = "bmp-2-4bpp v$version_number\nConvert bitmap to SEGA Saturn 4bpp graphic format using an external palette.\n\nWritten by Derek Pascarella (ateam)";
my $usage_deatils = "Usage:\nbmp-2-4bpp <BITMAP_FILE> <PALETTE_FILE> <PALETTE_INDEX> <WIDTH> <HEIGHT>";

# Ensure proper input parameters.
if($input_file eq "" || $palette_file eq "" || $width eq "" || $height eq "" || $palette_index eq "")
{
	die "\n$usage_header\n\nOne or more input parameters missing...\n\n$usage_deatils\n\n";
}
elsif(!-e $input_file || !-R $input_file || !-e $palette_file || !-R $palette_file)
{
	die "\n$usage_header\n\nCould not find or read $input_file or $palette_file...\n\n$usage_deatils\n\n";
}
elsif($width eq "" || $height eq "" || $palette_index eq "" || $width !~ /^[+-]?\d+$/ || $height !~ /^[+-]?\d+$/ || $palette_index !~ /^[+-]?\d+$/)
{
	die "\n$usage_header\n\nWidth, height, and palette index parameters must be whole numbers...\n\n$usage_deatils\n\n";
}

# Construct output filename.
(my $output_file = $input_file) =~ s/\.BMP/.bin/gi;

# Status message.
print "\nbmp-2-15bpp v$version_number\n";
print "Convert bitmap to SEGA Saturn 4bpp graphic format using an external palette.\n\n";
print "Written by Derek Pascarella (ateam)\n\n";
print "Converting $input_file to $output_file...\n\n";

# Parse the RIFF palette file.
open(my $palette_in, '<:raw', $palette_file) or die "Error: Can't open palette file $palette_file: $!\n\n";
read($palette_in, my $header, 24) == 24 or die "Error: Palette file is too short or invalid.\n\n";

# Calculate the offset for the selected palette (16 colors × 4 bytes each).
my $palette_size = 16 * 4;
my $palette_offset = 24 + ($palette_index * $palette_size);
seek($palette_in, $palette_offset, 0) or die "Error: Failed to seek to palette index $palette_index.\n\n";

# Read the selected palette.
my @palette;

for my $i (0 .. 15)
{
	read($palette_in, my $entry, 4) == 4 or die "Error: Palette file does not contain enough color entries for index $palette_index.\n\n";
	my ($r, $g, $b, $reserved) = unpack("C4", $entry);
	
	# Ignore the reserved byte.
	push @palette, [$r, $g, $b];
}

close($palette_in);

# Status message.
print "Loaded palette index $palette_index...\n";

for my $i (0 .. $#palette)
{
	my ($r, $g, $b) = @{$palette[$i]};
	
	printf(" - Color %2d: R=%3d G=%3d B=%3d\n", $i, $r, $g, $b);
}

# Open the BMP file.
open(my $bmp_in, '<:raw', $input_file) or die "Error: Can't open $input_file: $!\n\n";
read($bmp_in, my $bmp_header, 54) == 54 or die "Error: BMP file is too short or invalid.\n\n";

# Determine orientation from BMP header height (offset 22 is height, signed 32-bit).
my $header_height = unpack("l", substr($bmp_header, 22, 4));
my $is_top_down = ($header_height < 0) ? 1 : 0;

# Read BMP pixel data.
my $pixel_data_offset = unpack("V", substr($bmp_header, 10, 4));
seek($bmp_in, $pixel_data_offset, 0) or die "Error: Failed to seek to BMP pixel data.\n\n";
read($bmp_in, my $pixel_data, $width * $height * 3) == $width * $height * 3 or die "Error: BMP file does not contain enough pixel data.\n\n";
close($bmp_in);

# Open the output file for writing.
open(my $output, '>:raw', $output_file) or die "Error: Can't open $output_file: $!\n\n";

# Convert BMP to 4bpp using the selected palette.
for my $y (0 .. $height - 1)
{
	# Use correct detected image orientation.
	my $line_y = $is_top_down ? $y : ($height - 1 - $y);

	for my $x (0 .. ($width / 2) - 1)
	{
		my $pixel1_offset = ($line_y * $width + $x * 2) * 3;
		my $pixel2_offset = $pixel1_offset + 3;
		my ($b1, $g1, $r1) = unpack("C3", substr($pixel_data, $pixel1_offset, 3));
		my ($b2, $g2, $r2) = unpack("C3", substr($pixel_data, $pixel2_offset, 3));

		# Find closest palette indices for the two pixels.
		my $index1 = find_closest_palette_index($r1, $g1, $b1, \@palette);
		my $index2 = find_closest_palette_index($r2, $g2, $b2, \@palette);

		# Pack the two indices into one byte and write to the output file.
		print $output pack("C", ($index1 << 4) | $index2);
	}
}

# Close output file.
close($output);

# Status message.
print "\nComplete!\n\n";

# Subroutine to find the closest color in the palette.
sub find_closest_palette_index
{
	my ($r, $g, $b, $palette) = @_;
	my $closest_index = 0;
	my $closest_distance = 1_000_000;

	for my $i (0 .. $#$palette)
	{
		my ($pr, $pg, $pb) = @{$palette->[$i]};
		my $distance = ($r - $pr) ** 2 + ($g - $pg) ** 2 + ($b - $pb) ** 2;
		
		if($distance < $closest_distance)
		{
			$closest_index = $i;
			$closest_distance = $distance;
		}
	}
	
	return $closest_index;
}