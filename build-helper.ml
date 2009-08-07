let releases = [ (* Add new releases to the front *)
  "8.1.1", 275
]

#load "unix.cma"

let sys cmd =
  let chan = Unix.open_process_in cmd in
  let line = input_line chan in
  let exitStatus = Unix.close_process_in chan in
  if exitStatus <> 0 then failwith (Printf.sprintf "Command '%s' failed with exit status %d" cmd exitStatus);
  line

let is_macos = Sys.os_type = "Unix" && sys "uname" = "Darwin"

let sh cmd =
  print_endline cmd;
  let result = Sys.command cmd in
  if result <> 0 then failwith (Printf.sprintf "Command failed with exit code %d" result)

let rm_Rf_cmd = match Sys.os_type with "Win32" -> "rmdir /s /q " | _ -> "rm -Rf "
let rm_Rf dir = sh (rm_Rf_cmd ^ dir)

let make_cmd = match Sys.os_type with "Win32" -> "nmake" | _ -> "make"

let (create_zip_cmd, zipext) =
  match Sys.os_type with
    "Win32" -> ("7z a", ".zip")
  | _ -> ("tar czf", ".tar.gz")

let create_zip zippath dirname = sh (Printf.sprintf "%s %s %s" create_zip_cmd zippath dirname)

let os_suffix = if is_macos then "-macos" else ""

let (//) x y = Filename.concat x y

let () =
  begin match Sys.argv with
    [| _; "--caller=build.ml" |] -> ()
  | _ -> failwith "Please do not run this script directly; run build.ml instead."
  end;
  let (release, revision) = List.hd releases in
  if Sys.file_exists "exportdir" then rm_Rf "exportdir";
  sh (Printf.sprintf "svn export https://aramis.cs.kuleuven.be/svn/verifast/verifast/trunk@%d exportdir" revision);
  Sys.chdir "exportdir";
  Sys.chdir "src";
  let mac_flag = if is_macos then "MAC=yes" else "" in
  sh (Printf.sprintf "%s %s VFVERSION=%s release" make_cmd mac_flag release);
  let releasename = Printf.sprintf "verifast-%s%s" release os_suffix in
  let zipname = releasename ^ zipext in
  let zippath = ".." // ".." // zipname in
  if Sys.file_exists zippath then Sys.remove zippath;
  create_zip zippath releasename