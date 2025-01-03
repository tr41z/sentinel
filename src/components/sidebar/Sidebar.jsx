import {
  ArrowRightIcon,
  ArrowLeftIcon,
  HomeIcon,
  CogIcon,
  EllipsisVerticalIcon,
  WifiIcon,
  BellAlertIcon,
  CodeBracketIcon,
} from "@heroicons/react/24/outline";
import { useState } from "react";
import SidebarItem from "./SidebarItem";
import { useLocation } from "react-router-dom"; 

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
      icon: <CodeBracketIcon />,
      text: "Inspector",
      subMenu: [
        {
          icon: <WifiIcon />,
          text: "Inspect Flows",
          to: "/inspector/view",
        },
        {
          icon: <BellAlertIcon />,
          text: "Alerts",
          to: "/inspector/alerts",
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