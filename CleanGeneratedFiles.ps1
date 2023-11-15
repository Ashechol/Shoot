$RemoveList = "Binaries", "Saved", "Intermediate"

foreach ($item in $RemoveList)
{
    Remove-Item -Path ".\$item" -Recurse -Force
}