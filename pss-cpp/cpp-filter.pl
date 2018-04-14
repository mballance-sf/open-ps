#!/usr/bin/perl

$infile = $ARGV[0];
$outfile = $ARGV[1];

#print "infile=$infile outfile=$outfile\n";

if ($infile eq "") {
	die "no infile argument specified";
}

if ($outfile eq "") {
	die "no outfile argument specified";
}

open(my $in, "<$infile") || die "failed to open infile $infile";
open(my $out, ">$outfile") || die "failed to opne outfile $outfile";

$prev_template_var="";
$next_template_var="";

while (<$in>) {
	my $line = $_;
	
	if ($line =~ /class .*{/) {
#		print "match class...{\n";
		if ($prev_template_var eq "") {
			$line =~ s/class ([a-zA-Z][a-zA-Z0-9_]*).*{/class $1 : public ${1}_impl {/g;
		} else {
			$line =~ s/class ([a-zA-Z][a-zA-Z0-9_]*).*{/class $1 : public ${1}_impl<$prev_template_var> {/g;
		}
	} elsif ($line =~ /template<class/) {
		$next_template_var = $line;
		chomp $next_template_var;
		$next_template_var =~ s/^.*class ([a-zA-Z]).*$/$1/g;
		print "next_template_var=$next_template_var\n";
	} elsif ($line =~ /pragma once/) {
		$line = $line . "#include \"pss_impl.h\"\n";
	} elsif ($line =~ /pss\/detail/) {
#		print "found include pss/detail\n";
		$line = "";
	} elsif ($line =~ /\.t/) {
		$line =~ s/pss\/timpl\/([a-zA-Z][a-zA-Z0-9_]*).t/${1}.hpp/g;
	}
	$prev_template_var = $next_template_var;
#	print "LINE: $line";

	$line =~ s/detail::AlgebExpr/impl::expr/g;
	$line =~ s/detail::ExecStmt/impl::exec_stmt/g;

	print $out $line;
}

close($in);
close($out);

