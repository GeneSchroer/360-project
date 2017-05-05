unless ($ARGV[0]) {
	die "No input supplied\n";
}
$ARGV[0] =~ /-> (\/.*$)/;
print "$1\n";
