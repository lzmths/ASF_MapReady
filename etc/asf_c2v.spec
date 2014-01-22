Summary: ASF Convert to Vector
Name: asf_c2v
Version: %{asftoolsversion}
Release: %{asftoolsbuildnumber}
License: BSD
Group: Applications/Scientific
URL: http://www.asf.alaska.edu
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Prefix: /usr/local
Requires: bison

%description
The ASF MapReady Remote Sensing Toolkit is a set of tools for
processing SAR data, including importing the raw CEOS data,
geocoding to various map projections, terrain correction,
as well as exporting to various common formats including
JPEG and GeoTIFF.

%prep
echo Executing: %%prep
%setup -DT -n asf_tools
 
%build
echo Executing: %%build
cd $RPM_BUILD_DIR/asf_tools
./configure --prefix=$RPM_BUILD_ROOT/usr/local
make c2v

%install
echo Executing: %%install
rm -rf $RPM_BUILD_ROOT
cd $RPM_BUILD_DIR/asf_tools
make install
rm -rf $RPM_BUILD_ROOT/usr/local/lib

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(666,root,root,777)
%attr(-,root,root) /usr/local/bin
/usr/local/share

