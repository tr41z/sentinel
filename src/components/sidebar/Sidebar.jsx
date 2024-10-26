import {
  ArrowRightIcon,
  ArrowLeftIcon,
  HomeIcon,
  CogIcon,
  UserIcon,
  EllipsisVerticalIcon,
} from "@heroicons/react/24/outline";
import { useState } from "react";
import SidebarItem from "./SidebarItem";
import { useLocation } from "react-router-dom"; 

// This sidebar component is for both mobile and desktop
function Sidebar({ children, expanded, setExpanded }) {
  return (
    <div className="relative z-50">
      <div
        className={`fixed inset-0 -z-10 block bg-gray-400  ${
          expanded ? "block sm:hidden" : "hidden"
        }`}
      />
      <aside
        className={`box-border h-screen transition-all ${
          expanded ? "w-5/6 sm:w-64" : "w-0 sm:w-20"
        }`}
      >
        <nav className="flex h-full flex-col border-r bg-white shadow-sm">
          <div className="flex items-center justify-between p-4 pb-2">
            <p
              className={`text-lg font-light text-main tracking-widest ${
                expanded ? "w-32" : "hidden"
              }`}
            >
              sentinellogo
            </p>
            <div className={`${expanded ? "" : "hidden sm:block"}`}>
              <button
                onClick={() => setExpanded((curr) => !curr)}
                className="rounded-lg bg-gray-50 p-1.5 hover:bg-gray-100"
              >
                {expanded ? (
                  <ArrowRightIcon className="h-6 w-6" />
                ) : (
                  <ArrowLeftIcon className="h-6 w-6" />
                )}
              </button>
            </div>
          </div>
          <ul className="flex-1 px-3">{children}</ul>
          <div className="flex border-t p-3">
            <img
              src="https://ui-avatars.com/api/?background=560bad&color=ebebeb&bold=true&name=John+Doe"
              alt=""
              className="h-10 w-10 rounded-md"
            />
            <div
              className={`
              flex items-center justify-between
              overflow-hidden transition-all ${expanded ? "ml-3 w-52" : "w-0"}
          `}
            >
              <div className="leading-4">
                <h4 className="font-semibold">John Doe</h4>
                <span className="text-xs text-gray-600">johndoe@gmail.com</span>
              </div>
              <EllipsisVerticalIcon className="h-6 w-6" />
            </div>
          </div>
        </nav>
      </aside>
    </div>
  );
}

export default function MakeSidebar() {
  const [expanded, setExpanded] = useState(true);
  const location = useLocation();

  const navBarItems = [
    {
      icon: <HomeIcon />,
      text: "Home",
      to: "/",
    },
    {
      icon: <UserIcon />,
      text: "Profile",
      subMenu: [
        {
          icon: <UserIcon />,
          text: "View Profile",
          to: "/profile/view", 
        },
        {
          icon: <CogIcon />,
          text: "Settings",
          to: "/profile/settings", 
        },
      ],
    },
    {
      icon: <CogIcon />,
      text: "Settings",
      to: "/settings",
    },
  ];

  return (
    <Sidebar expanded={expanded} setExpanded={setExpanded}>
      {navBarItems.map((item, index) => (
        <SidebarItem
          key={index}
          expanded={expanded}
          {...item}
          active={location.pathname === item.to}
        />
      ))}
    </Sidebar>
  );
}