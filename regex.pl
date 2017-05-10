unless ($ARGV[0]) {
	exit;
}
$ARGV[0] =~ /-> (\/.*$)/;
print "$1\n";
